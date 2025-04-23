#include "CollisionManager.h"
#include "Player.h"
#include "StageManager.h"
#include "Components.h"

CollisionManager::CollisionManager(std::shared_ptr <StageManager>& stage) :
	_pStage(stage)
{
}

CollisionManager::~CollisionManager()
{
}

ColResult CollisionManager::PlayerCollision(std::vector<std::shared_ptr<Player>>& player)
{
	ColResult result;

	for (int i = 0; i < player.size(); i++) {
		// 移動ベクトルを初期化する
		_vResultMove = 0;

		// カプセルとカプセルの当たり判定をとる

		float moveDist = (player[i]->_capsuleData.FrontPointA - player[i]->_capsuleData.PointA).Length();

		for (int n = 0; n < player.size(); n++) {

			// プレイヤーが自身と当たり判定をとらないようにする
			if (n != i) {
				// カプセルの線分同士の距離を求める
				float dist = DistanceLineToLine(player[i]->_capsuleData, player[n]->_capsuleData);

				// 二つのカプセルの半径を足した値より距離が短ければ当たっている
				if (dist <= player[i]->_capsuleData.Radius + player[n]->_capsuleData.Radius) {

					if (moveDist >= (player[n]->_capsuleData.FrontPointA - player[n]->_capsuleData.PointA).Length()) {

						if (player[n]->GetGroundFlag()) {
							float aaaa = (player[n]->_capsuleData.FrontPointA - player[n]->_capsuleData.PointA).Length();
							Vec3 vec = CreateMoveVectorCapsule(player[i]->_capsuleData, player[n]->_capsuleData, dist);
							_vResultMove += vec;

							player[i]->_capsuleData.PointA += vec;
							player[i]->_capsuleData.PointB += vec;
						}
					}
				}
			}
		}

		// カプセルとステージの当たり判定を取る

		Vec3 max, min;// 最大座標,最小座標

		for (int a = 0; a < BLOCK_NUM_X; a++) {
			for (int b = 0; b < BLOCK_NUM_Y; b++) {
				for (int c = 0; c < BLOCK_NUM_Z; c++) {

					// 対象のボックスが存在した場合のみ判定する
					if (_pStage->GetStageInfo(a, b, c) != 0) {

						// ボックスの最大座標と最小座標を求める
						Vec3 vec = { a,b,c };
						max = Vec3{ vec * _pStage->GetConstantInt("BLOCK_SIZE") };
						min = Vec3{ max - _pStage->GetConstantInt("BLOCK_SIZE") };

						// めり込んでいるかどうか判定する
						if (CollisionBoxCapsule(max, min, player[i]->_capsuleData)) {

							// ずらす分の移動ベクトルを作成
							Vec3 vec = CreateMoveVectorBox(max, min, player[i]->_capsuleData);

							// 配列に保存
							_vResultMove += vec;

							player[i]->_capsuleData.PointA += vec;
							player[i]->_capsuleData.PointB += vec;
						}
					}
				}
			}
		}
		// 最終的な移動ベクトルを返す
		result.vec[i] = _vResultMove;
	}

	return result;
}

bool CollisionManager::CollisionBoxCapsule(Vec3 max, Vec3 min, CapsuleData data)
{
	// カプセルの線分とAABBとの最短距離の２乗を計算
	float distance = DistancePointToBox(data, max, min);

	// 距離がカプセルの半径以内かどうかを判定
	return distance <= (data.Radius * data.Radius);
}

float CollisionManager::DistancePointToBox(CapsuleData data, Vec3 max, Vec3 min)
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

Vec3 CollisionManager::ClosetPointBox(Vec3 max, Vec3 min, Vec3 point)
{
	// 最近接点
	Vec3 closestPoint;

	// xyz軸について判定
	closestPoint.x = (std::max)(min.x, (std::min)(point.x, max.x));
	closestPoint.y = (std::max)(min.y, (std::min)(point.y, max.y));
	closestPoint.z = (std::max)(min.z, (std::min)(point.z, max.z));

	return closestPoint;
}

Vec3 CollisionManager::CreateMoveVectorBox(Vec3 max, Vec3 min, CapsuleData data)
{
	// 当たる直前にカプセルがボックスに対してどの位置にいたのかを調べる
	// ボックスの中で直前のカプセルの中心に最も近い点を算出
	Vec3 closestA, closestB, unit;

	ColDataT colData;

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

			return colData.moveVec;

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
			return colData.moveVec;



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

			return colData.moveVec;

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
			return colData.moveVec;
		};

	}
}

float CollisionManager::DistanceLineToLine(CapsuleData dataA, CapsuleData dataB)
{
	// 線分 dataA の方向ベクトル
	Vec3 d1 = dataA.PointA - dataA.PointB;
	// 線分 dataB の方向ベクトル
	Vec3 d2 = dataB.PointA - dataB.PointB;
	// 2つの線分の始点間のベクトル
	Vec3 r = dataA.PointA - dataB.PointA;

	// 各種スカラー値の計算
	float a = d1.dot(d1); // d1 の長さの2乗
	float e = d2.dot(d2); // d2 の長さの2乗
	float f = d2.dot(r);   // d2 と r の内積

	float s, t;
	const float epsilon = static_cast<float>(1e-6); // 許容誤差
	float c = d1.dot(r);  // d1 と r の内積
	float b = d1.dot(d2); // d1 と d2 の内積
	float denom = a * e - b * b; // 分母の計算

	// s の計算（線分 dataA 上の最近接点のパラメータ）
	if (denom != 0) {
		s = (b * f - c * e) / denom;
	}
	else {
		s = 0.0f; // 平行な場合
	}
	// s を [0,1] の範囲にクランプ
	s = (std::max)(0.0f, (std::min)(1.0f, s));

	// t の計算（線分 dataB 上の最近接点のパラメータ）
	t = (b * s + f) / e;

	// t の範囲制約処理
	if (t < 0.0f) {
		t = 0.0f;
		s = (std::max)(0.0f, (std::min)(1.0f, -c / a)); // s を再計算
	}
	else if (t > 1.0f) {
		t = 1.0f;
		s = (std::max)(0.0f, (std::min)(1.0f, (b - c) / a)); // s を再計算
	}

	// 線分上の最近接点の計算
	Vec3 closestPoint1 = dataA.PointA + d1 * s;
	Vec3 closestPoint2 = dataB.PointA + d2 * t;

	// 2つの最近接点の距離を返す
	return (closestPoint1 - closestPoint2).Length();
}

Vec3 CollisionManager::CreateMoveVectorCapsule(CapsuleData dataA, CapsuleData dataB, float dist)
{
	// 今の座標から前の座標までの単位ベクトルを求める
	//Vec3 unit = (Vec3{ dataA.FrontPointA.x,0.0f,dataA.FrontPointA.z } - Vec3{ dataA.PointA.x,0.0f, dataA.PointA.z }).GetNormalized();
	Vec3 unit = (dataA.FrontPointA - dataB.PointA).GetNormalized();

	// めり込んでいる分の距離を求める
	float full = dataA.Radius + dataB.Radius;
	float cave = full - dist;

	// 移動ベクトルを求める
	return unit * cave;
}
