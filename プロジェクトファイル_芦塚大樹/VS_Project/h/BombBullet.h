#pragma once
#include "BulletBase.h"
#include <memory>

class BulletManager;
class MapCollisionManager;
class BombBullet:
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
	BombBullet(Vec3 dist, Vec3 pos, std::shared_ptr<MapBulletCollisionManager>& col, BulletManager& mgr, int plNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BombBullet();
	
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
	// コリジョンマネージャーの参照
	std::shared_ptr<MapBulletCollisionManager>& _collManager;

	// バレットマネージャーの参照
	BulletManager& _bulletManager;

	// 爆発エフェクト
	std::shared_ptr<MyEffect> _explosionEffect;

	// 爆発エフェクトを描画するためのフレームカウンタ
	int _frame;
};

