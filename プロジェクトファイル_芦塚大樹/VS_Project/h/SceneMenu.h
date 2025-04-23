#pragma once
#include "SceneBase.h"
#include <memory>

class MenuManager;
class SceneMenu :
    public SceneBase
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="padNum">メニューを開いたパッド</param>
    SceneMenu(int padNum);

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~SceneMenu();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() const;

private:

    // 関数ポインタ
    using _updateFunc_t = void (SceneMenu::*)();
    using _drawFunc_t = void (SceneMenu::*)() const;

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

    // メニューマネージャーのポインタ
    std::shared_ptr<MenuManager> _manager;

    // 操作するパッド
    int _padNum;

    // フェード用フレームカウンタ
    int _frame;
};

