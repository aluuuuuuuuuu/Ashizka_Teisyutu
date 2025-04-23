#include "MapBulletCollisionManager.h"
#include "StageManager.h"
#include "Vec3.h"
#include "BulletManager.h"
#include "DxLib.h"

MapBulletCollisionManager::MapBulletCollisionManager(std::shared_ptr<StageManager>& stage) :
	_stage(stage)
{
}

MapBulletCollisionManager::~MapBulletCollisionManager()
{
}

bool MapBulletCollisionManager::CollisionBullet(Vec3 pos, float radius, int bullet)
{
	Vec3 max, min;// 最大座標,最小座標
	// カプセルとステージの当たり判定を取る
	for (int a = 0; a < BLOCK_NUM_X; a++) {
		for (int b = 0; b < BLOCK_NUM_Y; b++) {
			for (int c = 0; c < BLOCK_NUM_Z; c++) {

				// 対象のボックスが存在した場合のみ判定する
				if (_stage->GetStageInfo(a, b, c) != 0) {

					// ボックスの最大座標と最小座標を求める
					max = Vec3{ static_cast<float>(a * 10),static_cast<float>(b * 10),static_cast<float>(c * 10) };
					min = Vec3{ max - 10 };

					// めり込んでいるかどうか判定する
					if (CollisionSphire(max, min, pos, radius)) {

						// 弾ごとの処理を行う
						switch (bullet)
						{
						case NORMAL_BULLET:
							// 当たったマスを削除する
							_stage->DeleteBox(a, b, c);
							break;
						case GRAPPLER_BULLET:
							// マップには影響を与えない
							break;
						case BOMB_BULLET:
							// 当たったマスの周囲も削除する
							_stage->DeleteBox(a, b, c);
							_stage->DeleteBox(a, b - 1, c);

							_stage->DeleteBox(a - 1, b, c);
							_stage->DeleteBox(a + 1, b, c);
							_stage->DeleteBox(a, b, c + 1);
							_stage->DeleteBox(a, b, c - 1);

							break;
						default:
							break;
						}

						// 当たったと返す
						return true;

					}
				}
			}
		}
	}

	return false;
}

bool MapBulletCollisionManager::CollisionSphire(Vec3 max, Vec3 min, Vec3 pos, float radius)
{
	// カプセルの座標とAABBとの最短距離の2乗を計算
	float distance = DistancePointToBox(max, min, pos, radius);

	// 距離がカプセルの半径以内かどうかを判定
	return distance <= radius;
}

float MapBulletCollisionManager::DistancePointToBox(Vec3 max, Vec3 min, Vec3 pos, float radius)
{
	// AABBに線分が含まれているかチェックする
	Vec3 closestPoint = ClosestPointBox(max, min, pos);

	// 座標がAABB内にある場合
	if (closestPoint.x == pos.x &&
		closestPoint.y == pos.y &&
		closestPoint.z == pos.z) {
		return 0.0f;	// 距離はゼロ
	}

	// 座標からAABBへの距離を計算
	float dist = (pos - closestPoint).SqLength();

	return dist;
}

Vec3 MapBulletCollisionManager::ClosestPointBox(Vec3 max, Vec3 min, Vec3 pos)
{
	// 最近接点
	Vec3 closestPoint;

	// xyz軸について判定
	closestPoint.x = (std::max)(min.x, (std::min)(pos.x, max.x));
	closestPoint.y = (std::max)(min.y, (std::min)(pos.y, max.y));
	closestPoint.z = (std::max)(min.z, (std::min)(pos.z, max.z));

	return closestPoint;
}
