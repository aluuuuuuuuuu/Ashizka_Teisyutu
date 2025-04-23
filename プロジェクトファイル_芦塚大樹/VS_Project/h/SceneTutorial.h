#pragma once
#include "SceneBase.h"
#include <memory>

class TutorialManager;
class SceneTutorial :
    public SceneBase
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneTutorial();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneTutorial();

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
    using _updateFunc_t = void (SceneTutorial::*)();
    using _drawFunc_t = void (SceneTutorial::*)() const;

    // 任意のアップデート関数変数、ドロー関数変数
    _updateFunc_t _updateFunc = nullptr;
    _drawFunc_t  _drawFunc = nullptr;

    void NormalUpdate();

    void NormalDraw() const;

    void FadeInUpdate();

    void FadeOutUpdate();

    void FadeInDraw() const;

    void FadeOutDraw() const;

    // チュートリアルマネージャーポインタ
    std::shared_ptr<TutorialManager> _pManager;


    // フレームカウンタ
    int _frame;

};

