#include "StageCollisionManager.h"
#include "StageManager.h"
#include <algorithm>
#include "Vec2.h"

StageCollisionManager::StageCollisionManager(std::shared_ptr<StageManager>& stage) :
	_pStage(stage)
{
	
}

StageCollisionManager::~StageCollisionManager()
{
}

bool CompareByDist(const ColData& a, const ColData& b)
{
	return a.dist < b.dist; // value を基準に小さい順に並べ替える
}

Vec3 StageCollisionManager::CapsuleCollision(CapsuleData data)
{
	//testFlag = false;
	// 移動ベクトルを初期化する
	_vResultMove = 0;
	_vAllColldata.clear();
	_vAllMove.clear();

	Vec3 max, min;// 最大座標,最小座標

	// カプセルとステージの当たり判定を取る
	for (int a = 0; a < BLOCK_NUM_X; a++) {
		for (int b = 0; b < BLOCK_NUM_Y; b++) {
			for (int c = 0; c < BLOCK_NUM_Z; c++) {

				// 対象のボックスが存在した場合のみ判定する
				if (_pStage->GetStageInfo(a, b, c) != 0) {

					// ボックスの最大座標と最小座標を求める
					Vec3 vec = { a,b,c };
					max = Vec3{ vec * _pStage->GetConstantInt("BLOCK_SIZE")};
					min = Vec3{ max - _pStage->GetConstantInt("BLOCK_SIZE") };

					// めり込んでいるかどうか判定する
					if (CollisionBoxCapsule(max, min, data)) {

						// ずらす分の移動ベクトルを作成
						ColData colData = CreateMoveVector(max, min, data);

						// 配列に保存
						_vAllColldata.push_back(colData);

						data.PointA += colData.moveVec;
						data.PointB += colData.moveVec;

						data.FrontPointA += colData.moveVec;
						data.FrontPointB += colData.moveVec;

					}
				}
			}
		}
	}	

	// 保存した全ての移動ベクトルを足して最終的な移動ベクトルを作成する
	for (auto& vec : _vAllColldata) {
		_vResultMove += vec.moveVec;
	}

	// 最終的な移動ベクトルを返す
	return _vResultMove;
}

bool StageCollisionManager::CollisionBoxCapsule(Vec3 max, Vec3 min, CapsuleData data)
{
	// カプセルの線分とAABBとの最短距離の２乗を計算
	float distance = DistancePointToBox(data, max, min);

	// 距離がカプセルの半径以内かどうかを判定
	return distance <= (data.Radius * data.Radius);
}

float StageCollisionManager::DistancePointToBox(CapsuleData data, Vec3 max, Vec3 min)
{
	// 他の値と比較するために最大値で初期化する
	float minDist = FLT_MAX;

	// AABB内に線分が含まれているかチェックする
	Vec3 closetPointStart = ClosetPointBox(max, min, data.PointA);
	Vec3 closetPointEnd = ClosetPointBox(max, min, data.PointB);

	// 線分の開始点がAABB内にある場合
	if (closetPointStart.x == data.PointA.x &&
		closetPointStart.y == data.PointA.y &&
		closetPointStart.z == data.PointA.z) {
		return 0.0f;	// 距離はゼロ
	}

	// 線分の終了点がAABBないにある場合
	if (closetPointEnd.x == data.PointB.x &&
		closetPointEnd.y == data.PointB.y &&
		closetPointEnd.z == data.PointB.z) {
		return 0.0f;	// 距離はゼロ
	}

	// 線分の各端点からAABBへの距離を計算
	float distStart = (data.PointA - closetPointStart).SqLength();
	float distEnd = (data.PointB - closetPointEnd).SqLength();

	// 最小の値を求める
	minDist = (std::min)(distStart, distEnd);

	return minDist;
}

Vec3 StageCollisionManager::ClosetPointBox(Vec3 max, Vec3 min, Vec3 point)
{
	// 最近接点
	Vec3 closestPoint;

	// xyz軸について判定
	closestPoint.x = (std::max)(min.x, (std::min)(point.x, max.x));
	closestPoint.y = (std::max)(min.y, (std::min)(point.y, max.y));
	closestPoint.z = (std::max)(min.z, (std::min)(point.z, max.z));
	//DrawSphere3D(closestPoint.VGet(), 2, 8, 0xffff00, 0xffff00, true);

	return closestPoint;
}

ColData StageCollisionManager::CreateMoveVector(Vec3 max, Vec3 min, CapsuleData data)
{
	// 当たる直前にカプセルがボックスに対してどの位置にいたのかを調べる
	// ボックスの中で直前のカプセルの中心に最も近い点を算出
	Vec3 closestA, closestB, unit;

	ColData colData;

	colData.max = max;
	colData.min = min;

	closestA = ClosetPointBox(max, min, data.FrontPointA);
	closestB = ClosetPointBox(max, min, data.FrontPointB);

	// より近い方を求める
	if ((closestA - data.FrontPointA).Length() < (closestB - data.FrontPointB).Length()) {

		// Aのほうが近い

		// 前フレームのAのy座標から半径分引いた値がmaxのyより上にあればyのみ計算yより下だったらxzのみ計算

		if (((data.FrontPointA.y - data.Radius) >= max.y)) {	// yのみ計算

			Vec3 target = Vec3{ data.PointA.x,max.y + data.Radius,data.PointA.z };

			unit = (target - closestA).GetNormalized();
			colData.dist = (target - closestA).Length();

			colData.moveVec = Vec3{ 0.0f,target.y - data.PointA.y,0.0f };

			return colData;

		}
		else {	// xzのみ計算

			// 当たる直前にプレイヤーが箱に対してどの位置にいたのかを調べる
			// 四角形の中で直前のプレイヤーの中心に最も近い点を計算
			float closestX = (std::max)(min.x, (std::min)(data.FrontPointA.x, max.x));
			float closestY = (std::max)(min.z, (std::min)(data.FrontPointA.z, max.z));

			// 最近点からプレイヤーの中心へ向かう単位ベクトルを作成する
			Vec3 center = (Vec3{ data.FrontPointA.x,0.0f,data.FrontPointA.z } - Vec3{ closestX,0.0f,closestY }).GetNormalized();

			// 最近点からプレイヤーの座標への距離を求める
			float length = (Vec3{ data.PointA.x,0.0f,data.PointA.z } - Vec3{ closestX,0.0f,closestY }).Length();

			colData.dist = length;

			// 半径から前座標への距離を引いた値がめり込んだ分の距離になる
			length = data.Radius - length;

			colData.moveVec = center * length;

			// 最近点からボックスの判定外への移動ベクトルを作成
			return colData;



		}
	}
	else {

		// Bのほうが近い
		// 前フレームのBのy座標に半径分足した値がminのyyより下にあればyのみ計算


		if (((data.FrontPointB.y + data.Radius) <= min.y)) {	// yのみ計算

			Vec3 target = Vec3{ data.PointB.x,min.y - data.Radius,data.PointB.z };

			unit = (target - closestB).GetNormalized();
			colData.dist = (target - closestB).Length();

			colData.moveVec = Vec3{ 0.0f,target.y + data.PointA.y,0.0f };

			return colData;

		}
		else {	// xzのみ計算

			// 当たる直前にプレイヤーが箱に対してどの位置にいたのかを調べる
			// 四角形の中で直前のプレイヤーの中心に最も近い点を計算
			float closestX = (std::max)(min.x, (std::min)(data.FrontPointA.x, max.x));
			float closestY = (std::max)(min.z, (std::min)(data.FrontPointA.z, max.z));

			// 最近点からプレイヤーの中心へ向かう単位ベクトルを作成する
			Vec3 center = (Vec3{ data.FrontPointA.x,0.0f,data.FrontPointA.z } - Vec3{ closestX,0.0f,closestY }).GetNormalized();

			// 最近点からプレイヤーの座標への距離を求める
			float length = (Vec3{ data.PointA.x,0.0f,data.PointA.z } - Vec3{ closestX,0.0f,closestY }).Length();

			colData.dist = length;

			// 半径から前座標への距離を引いた値がめり込んだ分の距離になる
			length = data.Radius - length;

			colData.moveVec = center * length;

			// 最近点からボックスの判定外への移動ベクトルを作成
			return colData;
		};

	}
}
