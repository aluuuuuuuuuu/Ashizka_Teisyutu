#pragma once
#include <memory>
#include "Vec3.h"

class BulletManager;
class PlayerManager;
class PlayerBulletCollisionManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="bull">バレットマネージャーの参照</param>
	/// <param name="pl">プレイヤーマネージャーの参照</param>
	PlayerBulletCollisionManager(std::shared_ptr<BulletManager>& bull,std::shared_ptr<PlayerManager>& pl);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBulletCollisionManager();

	/// <summary>
	/// マイフレーム弾とプレイヤーの当たり判定を取る
	/// </summary>
	void Update();

private:

	/// <summary>
	/// 最近接点を求める関数
	/// </summary>
	/// <param name="A">カプセルの線分のAポイント</param>
	/// <param name="B">カプセルの線分のBポイント</param>
	/// <param name="P">球の中心座標</param>
	/// <returns></returns>
	Vec3 closestPointOnSegment(Vec3& A, Vec3& B, Vec3& P);

	/// <summary>
	/// カプセルと球の当たり判定
	/// </summary>
	/// <param name="A">カプセルの線分のAポイント</param>
	/// <param name="B">カプセルの線分のBポイント</param>
	/// <param name="capsuleRadius">カプセルの半径</param>
	/// <param name="sphereCenter">球の中心座標</param>
	/// <param name="sphereRadius">球の半径</param>
	/// <returns>当たっていたらtrue</returns>
	bool capsuleSphereCollision(Vec3& A, Vec3& B, float capsuleRadius, Vec3& sphereCenter, float sphereRadius);

	// バレットマネージャーの参照
	std::shared_ptr<BulletManager>& _pBulletManager;

	// プレイヤーマネージャーの参照
	std::shared_ptr<PlayerManager>& _pPlayerManager;


};

