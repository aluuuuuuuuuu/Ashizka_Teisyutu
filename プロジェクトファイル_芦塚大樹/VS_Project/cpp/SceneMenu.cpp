#include "SceneMenu.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "Application.h"

SceneMenu::SceneMenu(int padNum) :
	_padNum(padNum),
	_frame(0)
{
	// 関数ポインタの初期化
	_updateFunc = &SceneMenu::NormalUpdate;
	_drawFunc = &SceneMenu::NormalDraw;

	// メニューマネージャーのポインタを作成
	_manager = std::make_shared<MenuManager>(_padNum);

	// 描画範囲と描画先の中心の初期化
	SetDrawArea(0, 0, Application::GetInstance().GetConstantInt("SCREEN_WIDTH"), Application::GetInstance().GetConstantInt("SCREEN_HEIGHT"));
	SetCameraScreenCenter(static_cast<float>(Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2), static_cast<float>(Application::GetInstance().GetConstantInt("SCREEN_HEIGHT") / 2));
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Update()
{
	(this->*_updateFunc)();
}

void SceneMenu::Draw() const
{
	(this->*_drawFunc)();
}

void SceneMenu::NormalUpdate()
{
	// マネージャーの更新処理
	_manager->Update();
}

void SceneMenu::NormalDraw() const
{
	// メニューマネージャーの描画処理
	_manager->Draw();
}
