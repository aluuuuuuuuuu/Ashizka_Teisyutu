#pragma once
#include <memory>
#include "Vec3.h"
#include "Components.h"
#include <vector>

struct ColDataT
{
	Vec3 max = 0;
	Vec3 min = 0;
	Vec3 moveVec = 0;
	float dist = 0;
};

struct ColResult
{
	Vec3 vec[4];
};

class Player;
class StageManager;
class CollisionManager
{
public:
	CollisionManager(std::shared_ptr <StageManager>& stage);
	~CollisionManager();

	ColResult PlayerCollision(std::vector<std::shared_ptr<Player>>& player);
private:

	/// <summary>
	/// ボックスとカプセルの当たり判定を取る
	/// </summary>
	/// <param name="max">ボックスの最大座標</param>
	/// <param name="min">ボックスの最小座標</param>
	/// <param name ="data">カプセルのデータ</param>
	/// <returns>あたっていればtrue</returns>
	bool CollisionBoxCapsule(Vec3 max, Vec3 min, CapsuleData data);
	
	/// <summary>
	/// 点とボックスの最短距離を求める
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <param name="max">ボックスの最大座標</param>
	/// <param name="min">ボックスの最小座標</param>
	/// <returns></returns>
	float DistancePointToBox(CapsuleData data, Vec3 max, Vec3 min);

	/// <summary>
	/// BOX上の最近接点を求める関数
	/// </summary>
	/// <param name="max">最大座標</param>
	/// <param name="min">最小座標</param>
	/// <param name="point">比較する座標</param>
	/// <returns>最近接点</returns>
	Vec3 ClosetPointBox(Vec3 max, Vec3 min, Vec3 point);

	/// <summary>
	/// ボックスにめり込んだ分のカプセルをずらす移動ベクトルを作成する
	/// </summary>
	/// <param name="max">ボックスの最大座標</param>
	/// <param name="min">ボックスの最小座標</param>
	/// <param name="data">カプセルのデータ</param>
	/// <returns>ずらす分の移動ベクトル</returns>
	Vec3 CreateMoveVectorBox(Vec3 max, Vec3 min, CapsuleData data);

	/// <summary>
	/// 線分同士の距離を求める
	/// </summary>
	/// <param name="dataA">当たり判定用データA</param>
	/// <param name="dataB">当たり判定用データB</param>
	/// <returns>距離</returns>
	float DistanceLineToLine(CapsuleData dataA, CapsuleData dataB);

	Vec3 CreateMoveVectorCapsule(CapsuleData dataA, CapsuleData dataB, float dist);

	// 最終的にずらす移動ベクトル
	Vec3 _vResultMove;

	// ステージマネージャーの参照
	std::shared_ptr<StageManager>& _pStage;
};

