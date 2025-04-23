#pragma once
#include <memory>
#include "Vec2.h"
#include "Constant.h"

constexpr int GUIDE_CAMERA = 0;
constexpr int GUIDE_MOVE = 1;
constexpr int GUIDE_JUMP = 2;
constexpr int GUIDE_SHOT = 3;
constexpr int GUIDE_BOMB = 4;
constexpr int GUIDE_GRAPPLE = 5;
constexpr int GUIDE_END = 6;
constexpr int GUIDE_RETURN = 7;

constexpr int MAX_STORY_FRAME = 180;

class WedgewormManager;
class StageManager;
class StageCollisionManager;
class BulletManager;
class MapBulletCollisionManager;
class PlayerManager;
class SkyDome;
class TutorialManager:
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TutorialManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TutorialManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// チュートリアルが終わったかどうかを返す
	/// </summary>
	/// <returns>true:終わっている</returns>
	bool GetEndFrag() const;


private:

	// 関数ポインタ
	using _updateFunc_t = void (TutorialManager::*)();
	using _drawFunc_t = void (TutorialManager::*)() const;

	// 任意のアップデート関数変数、ドロー関数変数
	_updateFunc_t _updateFunc = nullptr;
	_drawFunc_t  _drawFunc = nullptr;

	void FirstUpdate();

	void CameraUpdate();

	void MoveUpdate();

	void JumpUpdate();

	void ShotUpdate();

	void BombUpdate();

	void GrappleUpdate();

	void LastUpdate();

	void FirstDraw() const;

	void ShareDraw() const;

	void LastDraw() const; 

	/// <summary>
	/// クリアしたときの処理
	/// </summary>
	/// <returns>Aボタンが押されたらtrueを返す</returns>
	bool ClearFunction();

	// デフォルトの処理
	void DefaultUpdate();

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

	// フレームカウンタ
	int _frame;

	// チュートリアルのガイド文章画像ハンドル
	int _guideHandle[8];

	// ガイド文章の座標
	Vec2 _guidePos;

	// チュートリアルの進行度
	int _tutorialProgress;

	// クリア画像の座標
	Vec2 _clearPos;

	// クリア画像のハンドル
	int _clearHandle;

	// 既定の処理を行ったフラグ
	bool _endFrag;

	// クリア画像のScale
	double _clearScale;

	// ゲーム説明の画像ハンドル
	int _explainHandle[7];

	// 弾を撃った回数のカウント
	int _shotCount;

	// すべてのチュートリアルが終了したフラグ
	int _finishFrag;

	int _lastDrawGraph;

};
 