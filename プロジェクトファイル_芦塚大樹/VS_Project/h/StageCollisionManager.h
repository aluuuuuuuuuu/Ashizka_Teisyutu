#pragma once
#include <memory>
#include "Vec3.h"
#include "Components.h"

struct ColData
{
	Vec3 max = 0;
	Vec3 min = 0;
	Vec3 moveVec = 0;
	float dist = 0;
};

class StageManager;
class StageCollisionManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageCollisionManager(std::shared_ptr <StageManager>& stage);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageCollisionManager();

	
	/// <summary>
	/// カプセルとステージの当たり判定を取る
	/// </summary>
	/// <param name="data">当たり判定を取る対象のカプセルデータ</param>
	/// <returns>判定した結果ずらす移動ベクトル</returns>
	Vec3 CapsuleCollision(CapsuleData data);

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
	ColData CreateMoveVector(Vec3 max, Vec3 min, CapsuleData data);

	// 最終的にずらす移動ベクトル
	Vec3 _vResultMove;

	// 判定したデータ
	std::vector<ColData> _vAllColldata;

	// 移動ベクトル
	std::vector<Vec3> _vAllMove;

	// ステージマネージャーの参照
	std::shared_ptr<StageManager>& _pStage;


	bool testFlag = false;
};

