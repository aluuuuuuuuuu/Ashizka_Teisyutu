#pragma once
#include "SceneBase.h"
#include "PlayerManager.h"
#include <memory>
#include "Vec2.h"
#include "Constant.h"

constexpr int CHARACTOR_1 = 1;
constexpr int CHARACTOR_2 = 2;
constexpr int CHARACTOR_3 = 3;
constexpr int CHARACTOR_4 = 4;

class SkyDome;
class SelectUI;
class CharacterSelectManager;
class CharacterCard;
class SelectFinger;
class SceneSelect :
    public SceneBase,
	public Constant
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="num">プレイヤー数</param>
    SceneSelect(int num);

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~SceneSelect();

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
    using _updateFunc_t = void (SceneSelect::*)();
    using _drawFunc_t = void (SceneSelect::*)() const;

    // 任意のアップデート関数変数、ドロー関数変数
    _updateFunc_t _updateFunc = nullptr;
    _drawFunc_t  _drawFunc = nullptr;

	/// <summary>
	/// キャラクター選択時の更新処理
	/// </summary>
	void CharacterSelectUpdate();

	/// <summary>
	/// キャラクター選択時の描画処理
	/// </summary>
	void CharacterSelectDraw() const;

	/// <summary>
	/// フェードアウト更新処理
	/// </summary>
	void FadeOutUpdate();

	/// <summary>
	/// フェードアウト描画処理
	/// </summary>
	void FadeOutDraw() const;

	/// <summary>
	/// スライドイン更新処理
	/// </summary>
	void SlideInUpdate();

	/// <summary>
	/// スライドイン描画処理
	/// </summary>
	void SlideInDraw() const;

	/// <summary>
	/// スライドアウト更新処理
	/// </summary>
	void SlideOutUpdate();

	/// <summary>
	/// スライドアウト描画処理
	///	</summary>
	void SlideOutDraw() const;

	// キャラセレクトマネージャー
	std::shared_ptr<CharacterSelectManager> _pSelectManager;

	// スカイドームの参照
	std::shared_ptr<SkyDome> _pSkyDome;

	// フェード用フレームカウンタ
	int _frame;

	// ゲームシーンに渡すプレイヤーのデータ
	PlayerData _plData;

	// uiクラスポインタ
	std::shared_ptr<SelectUI> _pUi;

	// スライド画像
	int _slideHandle;

	// スライド画像の座標
	Vec2 _slidePos;

};

