#pragma once
#include "SceneBase.h"
#include <memory>
#include "PlayerManager.h"
#include "Constant.h"

class SkyDome;
class ResultCharacter;
class NumUtility;
class ResultUi;
class SceneResult :
	public SceneBase,
	public Constant
{
public:


	SceneResult(PlayerData data, int gameTime);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneResult();

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
	using _updateFunc_t = void (SceneResult::*)();
	using _drawFunc_t = void (SceneResult::*)() const;

	// 任意のアップデート関数変数、ドロー関数変数
	_updateFunc_t _updateFunc = nullptr;
	_drawFunc_t  _drawFunc = nullptr;

	/// <summary>
	/// 通常の更新処理
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// 通常の描画処理
	/// </summary>
	void NormalDraw() const;

	/// <summary>
	/// フェード用の更新処理
	/// </summary>
	void FadeOutUpdate();

	/// <summary>
	/// フェード用の描画処理
	/// </summary>
	void FadeOutDraw() const;

	// スカイドーム　
	std::shared_ptr<SkyDome> _pSkyDome;

	// 数字利用ポインタ
	std::shared_ptr<NumUtility> _pNum;

	// リザルトロゴポインタ
	std::shared_ptr<ResultUi> _pResultUi;

	// プレイヤーデータを保持する
	PlayerData _playerData;

	// キャラクター
	std::shared_ptr<ResultCharacter> _pCharacter;

	// フレームカウンタ
	int _frame;

	// 次にロードするシーン
	int _nextScene;
};

