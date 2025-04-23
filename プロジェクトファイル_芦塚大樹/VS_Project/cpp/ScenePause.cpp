#include "ScenePause.h"]
#include "SceneManager.h"
#include "DxLib.h"
#include "ScenePve.h"
#include "ScenePvp.h"
#include "Input.h"
#include "Application.h"

ScenePause::ScenePause(PlayerData data) :
	_data(data),
	_frame(Application::GetInstance().GetConstantInt("FRAME_NUM")),
	_textFrame(0)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/ScenePause.csv");

	// 画像のロード
	_graphHandle = LoadGraph("data/image/Pause.png");
	_textImageHandle = LoadGraph("data/image/PressAnyButton.png");

	_updateFunc = &ScenePause::FadeInUpdate;
	_drawFunc = &ScenePause::FadeDraw;
}

ScenePause::~ScenePause()
{
	DeleteGraph(_graphHandle);
	DeleteGraph(_textImageHandle);
}

void ScenePause::Update()
{
	(this->*_updateFunc)();
}

void ScenePause::Draw() const
{
	(this->*_drawFunc)();
}

void ScenePause::NormalUpdate()
{
	for (int num = 0; num <= _data.playerNum; num++) {
		if (Input::GetInstance().IsTrigger(INPUT_A, num)) {
			_updateFunc = &ScenePause::FadeOutUpdate;
			_drawFunc = &ScenePause::FadeDraw;
		}
	}

	// スタート指示を点滅させる
	if (_textFrame == Application::GetInstance().GetConstantInt("FRAME_NUM") * 2) {
		_textFrame++;
	}
	else if (_textFrame == 1) {
		_textFrame--;
	}
	else if (_textFrame % 2 == 0) {
		_textFrame += 2;
	}
	else {
		_textFrame -= 2;
	}
}

void ScenePause::NormalDraw() const
{
	// 操作説明画像の描画
	DrawGraph(0, 0, _graphHandle, true);

	// フェードしながら描画
	int alpha = static_cast<int>(255 * ((float)_textFrame / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// 下のテキスト画像の描画
	DrawRotaGraph(GetConstantInt("IMAGE_DRAW_X"), GetConstantInt("IMAGE_DRAW_Y"), 1.0f, 0.0f, _textImageHandle, true);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ScenePause::FadeInUpdate()
{
	NormalUpdate();

	_frame--;
	if (_frame == 0) {
		_updateFunc = &ScenePause::NormalUpdate;
		_drawFunc = &ScenePause::NormalDraw;
	}
}

void ScenePause::FadeOutUpdate()
{
	NormalUpdate();

	_frame++;
	if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM")) {
		// 次のシーンに移行する
		if (_data.playerNum == PLAYER_ONE) {
			_data.aiFlag = true;
			SceneManager::GetInstance().ChangeScene(std::make_shared<ScenePve>(_data));
		}
		else {
			SceneManager::GetInstance().ChangeScene(std::make_shared<ScenePvp>(_data));
		}
	}
}

void ScenePause::FadeDraw() const
{
	// 通常の描画
	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM")));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, 1980, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
