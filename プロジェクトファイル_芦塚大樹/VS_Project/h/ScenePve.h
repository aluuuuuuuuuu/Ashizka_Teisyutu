#pragma once
#include "SceneBase.h"
#include "Constant.h"
#include <memory>
#include <vector>

class ItemManager;
class PlayerBulletCollisionManager;
class BulletManager;
class NumUtility;
class GameFlowManager;
class WedgewormManager;
class SkyDome;
class PlayerManager;
class MapBulletCollisionManager;
class StageCollisionManager;
class StageManager;
struct PlayerData;
class ScenePve :
	public SceneBase,
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data">プレイヤーデータ</param>
	ScenePve(PlayerData data);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ScenePve();

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
	using m_updateFunc_t = void (ScenePve::*)();
	using m_drawFunc_t = void (ScenePve::*)() const;
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

	// ゲームフローマネージャー
	std::shared_ptr<GameFlowManager> _pGameFlowManager;

	// プレイヤーマネージャー
	std::shared_ptr<PlayerManager> _pPlayerManager;

	// ステージマネージャー
	std::shared_ptr<StageManager> _pStage;

	// バレットマネージャー
	std::shared_ptr<BulletManager> _pBulletManager;

	// バレットコリジョンマネージャー
	std::shared_ptr<MapBulletCollisionManager> _pBulletCollsionManager;

	// ステージコリジョンマネージャー
	std::shared_ptr<StageCollisionManager> _pStageCollisionManager;

	// プレイヤーバレットコリジョンマネージャー
	std::shared_ptr<PlayerBulletCollisionManager> _pPlayerBulletCollisionManager;

	// 禊虫マネージャー
	std::shared_ptr<WedgewormManager> _pWedgewormManager;

	// アイテムマネージャー
	std::shared_ptr<ItemManager> _pItemManager;

	// スカイドーム
	std::shared_ptr<SkyDome> _pSkyDome;

	// 数字ユーティリティ
	std::shared_ptr<NumUtility> _pNum;

	// フレームカウンタ
	int _frame;
};

