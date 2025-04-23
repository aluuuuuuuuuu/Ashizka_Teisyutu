#pragma once
#include "SceneBase.h"
#include <memory>
#include "Vec2.h"
#include "Constant.h"

class SkyDome;
class SelectModeUi;
class SceneSelectMode :
    public SceneBase,
    public Constant
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="slideInFlag">スライドインするフラグ</param>
    SceneSelectMode(bool slideInFlag);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneSelectMode();

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
    using _updateFunc_t = void (SceneSelectMode::*)();
    using _drawFunc_t = void (SceneSelectMode::*)() const;

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

    // UIポインタ
	std::shared_ptr<SelectModeUi> _pUi;

    // スカイドームポインタ
    std::shared_ptr<SkyDome> _pSkyDome;

    // スライド画像ハンドル
    int _slideHandle;

    // スライドイン画像の座標
    Vec2 _slidePos;

    // タイトルシーンに戻るフラグ
    bool _titleFrag;

    // セレクトシーンから戻ったフラグ
    bool _returnSelectFrag;

    // フレームカウンタ
    int _frame;
};

