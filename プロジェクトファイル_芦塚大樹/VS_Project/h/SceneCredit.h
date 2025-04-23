#pragma once
#include "SceneBase.h"
#include <memory>
#include "Vec2.h"
#include "Constant.h"

class CreditCharacter;
class SceneCredit :
    public SceneBase,
	public Constant
{
public:
    SceneCredit();

    virtual ~SceneCredit();

    void Update();

    void Draw() const;

private:

	// 関数ポインタ
	using _updateFunc_t = void (SceneCredit::*)();
	using _drawFunc_t = void (SceneCredit::*)() const;

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
	/// フェードイン更新処理
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// フェードアウト更新処理
	/// </summary>
	void FadeOutUpdate();

	/// <summary>
	/// フェード描画処理
	/// </summary>
	void FadeDraw() const;

	// クレジット画像ハンドル
	int _creditHandle;

	// クレジット表示Y座標
	int _creditY;

	// クレジットフレームカウンタ
	int _creditFrame;

	// クレジットキャラ
	std::shared_ptr<CreditCharacter> _pChar1;
	std::shared_ptr<CreditCharacter> _pChar2;
	std::shared_ptr<CreditCharacter> _pChar3;
	std::shared_ptr<CreditCharacter> _pChar4;

	// フェード用フレームカウンタ
	int _frame;
};

