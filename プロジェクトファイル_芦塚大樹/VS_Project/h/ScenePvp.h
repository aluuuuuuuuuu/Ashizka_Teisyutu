#pragma once
#pragma once
#include "SceneBase.h"
#include "Constant.h"
#include <memory>
#include <vector>

class ItemManager;
class PlayerBulletCollisionManager;
class NumUtility;
class GameFlowManager;
class WedgewormManager;
class SkyDome;
class PlayerManager;
class MapBulletCollisionManager;
class BulletManager;
class StageCollisionManager;
class StageManager;
struct PlayerData;
class ScenePvp :
	public SceneBase,
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data">プレイヤーデータ</param>
	ScenePvp(PlayerData data);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ScenePvp();

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
    using m_updateFunc_t = void (ScenePvp::*)();
    using m_drawFunc_t = void (ScenePvp::*)() const;
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

    /// <summary>
    /// フェードイン更新処理
    /// </summary>
    void FadeInUpdate();

    /// <summary>
    /// フェードイン描画処理
    /// </summary>
    void FadeInDraw() const;

    void MakeScreen();

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

    // プレイヤーバレットコリジョンマネージャー
    std::shared_ptr<PlayerBulletCollisionManager> _pPlayerBulletCollisionManager;

    // スカイドームポインタ
    std::shared_ptr<SkyDome> _pSkyDome;

    // 禊虫マネージャー
    std::shared_ptr<WedgewormManager> _pWedgewormManager;

    // アイテムマネージャー
    std::shared_ptr<ItemManager> _pItemManager;

    // ゲームフローマネージャー
    std::shared_ptr<GameFlowManager> _pGameFlowManager;

    // 数字クラスポインタ
    std::shared_ptr<NumUtility> _pNum;

    // フレームカウンタ
    int _frame;

    std::vector<int> _screen;
};

