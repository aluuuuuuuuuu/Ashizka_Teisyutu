#include "SceneTutorial.h"
#include "TutorialManager.h"
#include "SceneManager.h"
#include "SceneSelectMode.h"
#include "SoundManager.h"
#include "Application.h"
#include "Constant.h"

SceneTutorial::SceneTutorial():
	_frame(Application::GetInstance().GetConstantInt("FRAME_NUM"))
{
	// マネージャーのインスタンス作成
	_pManager = std::make_shared<TutorialManager>();

	// 関数ポインタの初期化
	_updateFunc = &SceneTutorial::FadeInUpdate;
	_drawFunc = &SceneTutorial::FadeInDraw;

	// BGMの再生
	SoundManager::GetInstance().StopBGM(BGM_OPENING);
	SoundManager::GetInstance().StartBGM(BGM_BATTLE);
}

SceneTutorial::~SceneTutorial()
{
	// ライトの削除
	DeleteLightHandleAll();
}

void SceneTutorial::Update()
{
	(this->*_updateFunc)();
}

void SceneTutorial::Draw() const
{
	(this->*_drawFunc)();
}

void SceneTutorial::NormalUpdate()
{
	// チュートリアルが終了したらフェードアウト処理に移行する
	if (_pManager->GetEndFrag()) {
		_updateFunc = &SceneTutorial::FadeOutUpdate;
		_drawFunc = &SceneTutorial::FadeOutDraw;
	}

	_pManager->Update();
}

void SceneTutorial::NormalDraw() const
{
	_pManager->Draw();
}

void SceneTutorial::FadeInUpdate()
{
	NormalUpdate();

	_frame--;
	if (_frame == 0) {
		_updateFunc = &SceneTutorial::NormalUpdate;
		_drawFunc = &SceneTutorial::NormalDraw;
	}
}

void SceneTutorial::FadeOutUpdate()
{
	_frame++;
	if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM")) {
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelectMode>(false));
	}
}

void SceneTutorial::FadeInDraw() const
{
	auto& app = Application::GetInstance();

	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / app.GetConstantInt("FRAME_NUM")));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, app.GetConstantInt("SCREEN_WIDTH"), app.GetConstantInt("SCREEN_HEIGHT"), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTutorial::FadeOutDraw() const
{
	auto& app = Application::GetInstance();

	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM")));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, app.GetConstantInt("SCREEN_WIDTH"), app.GetConstantInt("SCREEN_HEIGHT"), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
