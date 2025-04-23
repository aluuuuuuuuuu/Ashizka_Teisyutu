#pragma once
#include <memory>
#include "BulletBase.h"

class MyEffect;
class BulletManager;
class MapBulletCollisionManager;
class NormalBullet :
	public BulletBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dist">方向ベクトル</param>
	/// <param name="pos">発生する座標</param>
	/// <param name="col">コリジョンマネージャー</param>
	/// <param name="mgr">バレットマネージャー</param>
	/// <param name="plNum">プレイヤーナンバー</param>
	NormalBullet(Vec3 dist, Vec3 pos, std::shared_ptr<MapBulletCollisionManager>& col, BulletManager& mgr, int plNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~NormalBullet();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// プレイヤーと当たった時の処理
	/// </summary>
	void PlayerCollision();
private:

	/// <summary>
	/// 弾が何かに当たった時の処理
	/// </summary>
	void CollisionFunction();

	// コリジョンマネージャーの参照
	std::shared_ptr<MapBulletCollisionManager>& _collManager;

	// バレットマネージャーの参照
	BulletManager& _bulletManager;

	// フレームカウンタ
	int _frame;

	// ブロック破壊エフェクト
	std::shared_ptr<MyEffect> _destroyEffect;

};

