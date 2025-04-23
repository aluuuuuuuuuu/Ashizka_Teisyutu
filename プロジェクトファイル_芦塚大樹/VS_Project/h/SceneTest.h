#pragma once
#include "SceneBase.h"
#include "Constant.h"
#include <memory>
#include "Components.h"
#include <vector>
#include "PlayerManager.h"

class NumUtility;
class GameFlowManager;
class WedgewormManager;
class SkyDome;
class PlayerManager;
class MapBulletCollisionManager;
class BulletManager;
class StageCollisionManager;
class StageManager;
class SceneTest :
    public SceneBase,
    public Constant
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="data">プレイヤーデータ</param>
    SceneTest(PlayerData& data);

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~SceneTest();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw() const;

private:

    // 関数ポインタ
    using m_updateFunc_t = void (SceneTest::*)();
    using m_drawFunc_t = void (SceneTest::*)() const;
    m_updateFunc_t _updateFunc = nullptr;
    m_drawFunc_t _drawFunc = nullptr;

    /// <summary>
    /// 通常の更新処理
    /// </summary>
    void NormalUpdate();

    /// <summary>
    /// 通常の描画処理
    /// </summary>
    void NormalDraw() const;

    /// <summary>
    /// ゲーム終了時の更新処理
    /// </summary>
    void EndUpdate();

    /// <summary>
    /// ゲーム終了時の描画処理
    /// </summary>
    void EndDraw() const;

    // ステージポインタ
    std::shared_ptr<StageManager> _pStage;

    // ステージコリジョンマネージャポインタ
    std::shared_ptr<StageCollisionManager> _pStageCollisionManager;

    // バレットマネージャポインタ
    std::shared_ptr<BulletManager> _pBulletManager;

    // バレットコリジョンマネージャー
    std::shared_ptr<MapBulletCollisionManager> _pBulletCollsionManager;

    // プレイヤーマネージャー
    std::shared_ptr<PlayerManager> _pPlayerManager;

    // スカイドームポインタ
    std::shared_ptr<SkyDome> _pSkyDome;

    // 禊虫マネージャー
    std::shared_ptr<WedgewormManager> _pWedgewormManager;

    // ゲームフローマネージャー
    std::shared_ptr<GameFlowManager> _pGameFlowManager;

    // 数字クラスポインタ
    std::shared_ptr<NumUtility> _pNum;

};

