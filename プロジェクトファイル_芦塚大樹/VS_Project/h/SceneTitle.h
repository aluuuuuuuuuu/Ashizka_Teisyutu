#pragma once
#include "SceneBase.h"
#include <memory>
#include "Constant.h"
#include "Vec2.h"

class CreditCharacter;
class TitlePlayer;
class SkyDome;
class FallCharactor;
class AnyPushButton;
class Logo;
class Crown;
class SceneTitle :
	public SceneBase,
	public Constant
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneTitle(bool slidInFlag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle();

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
	using _updateFunc_t = void (SceneTitle::*)();
	using _drawFunc_t = void (SceneTitle::*)() const;

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
	/// スライドインの更新処理
	/// </summary>
	void SlideInUpdate();

	/// <summary>
	/// スライドアウトの更新処理
	/// </summary>
	void SlideOutUpdate();

	/// <summary>
	///	スライドインの描画処理
	/// </summary>
	void SlideInDraw() const;

	/// <summary>
	/// スライドアウトの描画処理
	/// </summary>
	void SlideOutDraw() const;

	/// <summary>
	/// フェードイン更新処理
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// フェードイン描画処理
	/// </summary>
	void FadeInDraw() const;

	/// <summary>
	/// 通常のフェードイン更新処理
	/// </summary>
	void NormalFadeInUpdate();

	/// <summary>
	/// 通常のフェードアウト更新処理
	/// </summary>
	void NormalFadeOutUpdate();

	/// <summary>
	/// 通常のフェード描画処理
	/// </summary>
	void NormalFadeDraw() const;

	// 王冠ポインタ
	std::shared_ptr<Crown> _pCrown;

	// ロゴポインタ
	std::shared_ptr<Logo> _pLogo;

	// 文章ポインタ
	std::shared_ptr<AnyPushButton> _pText;

	// 落下キャラクターポインタ
	std::shared_ptr<FallCharactor> _pFallCharactor;

	// スカイドームポインタ
	std::shared_ptr<SkyDome> _pSkyDome;

	// 両翼のモデル
	std::shared_ptr<TitlePlayer> _pModel1;
	std::shared_ptr<TitlePlayer> _pModel2;

	// フェード用フレームカウンタ
	int _frame;

	// スライド画像ハンドル
	int _slideHandle;

	// スライドイン画像の座標
	Vec2 _slidePos;

	// スライドインフラグ
	bool _selectDrawFlag;

	// セリフ画像ハンドル
	int _serihu;
	int _serihu2;
	int _serihu3;

	// ゲーム終了フラグ
	bool _endFrag;

	// ゲーム終了確認画像ハンドル
	int _endHandle;

	// ゲーム終了ボタンテキスト画像
	int _gameEndHandle;
};

