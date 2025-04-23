#pragma once
#include <memory>
#include "BulletBase.h"

class WedgewormManager;
class BulletManager;
class MapBulletCollisionManager;
class GrapplerBullet :
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
    GrapplerBullet(Vec3 dist, Vec3 pos, std::shared_ptr<MapBulletCollisionManager>& col, BulletManager& mgr, int plNum, std::shared_ptr<WedgewormManager>& wedge);

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~GrapplerBullet();

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

    // 禊虫マネージャーの参照
    std::shared_ptr<WedgewormManager>& _wedgeManager;

    int _frame;
};

