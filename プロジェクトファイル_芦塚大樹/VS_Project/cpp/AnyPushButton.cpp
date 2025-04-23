#include "AnyPushButton.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "Application.h"

AnyPushButton::AnyPushButton(SceneTitle& scene) :
	_frame(0)
{
	// 画像のロード
	_textHandle = LoadGraph("data/image/PressAnyButton.png");

	// 表示位置の設定
	_dispPos = Vec2{ scene.GetConstantInt("TEXT_POS_X"),scene.GetConstantInt("TEXT_POS_Y") };
}

AnyPushButton::~AnyPushButton()
{
	DeleteGraph(_textHandle);
}

void AnyPushButton::Update()
{
	// スタート指示を点滅させる
	if (_frame == Application::GetInstance().GetConstantInt("FRAME_NUM")) {
		_frame++;
	}
	else if (_frame == 1) {
		_frame--;
	}
	else if (_frame % 2 == 0) {
		_frame += 2;
	}
	else {
		_frame -= 2;
	}
}

void AnyPushButton::Draw() const
{
	// フェードしながら描画
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// 画像の描画
	DrawRotaGraph(_dispPos.intX(), _dispPos.intY(), 1.0f, 0.0f, _textHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
