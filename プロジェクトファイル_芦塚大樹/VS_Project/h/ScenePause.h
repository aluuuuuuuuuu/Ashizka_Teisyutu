#pragma once
#include "SceneBase.h"
#include "PlayerManager.h"
#include "Constant.h"

class ScenePause :
    public SceneBase,
    public Constant
{
public:


    ScenePause(PlayerData data);

    virtual ~ScenePause();

    void Update();

    void Draw() const;
private:

    // 関数ポインタ
    using m_updateFunc_t = void (ScenePause::*)();
    using m_drawFunc_t = void (ScenePause::*)() const;
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

    void FadeInUpdate();

    void FadeOutUpdate();

    void FadeDraw() const;

    PlayerData _data;

    int _frame;

    int _graphHandle;

    int _textImageHandle;

    int _textFrame;

};

