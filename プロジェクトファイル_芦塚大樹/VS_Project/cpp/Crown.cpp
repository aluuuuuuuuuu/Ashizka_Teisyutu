#include "Crown.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "Application.h"

Crown::Crown(SceneTitle& scene) :
	_flame1(0),
	_flame2(0),
	_flame1Flag(true),
	_blinkingFlag(false)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/Crown.csv");

	// 表示位置の設定
	_dispPos = Vec2{ scene.GetConstantInt("CROWN_POS_X"),scene.GetConstantInt("CROWN_POS_Y") };

	// 画像のロード
	{
		_crownHandle = LoadGraph("data/image/Crown.png");	// 王冠画像のロード
		_crownParticleHandle1 = LoadGraph("data/image/CrownParticle1.png");	// 王冠パーティクル1画像のロード
		_crownParticleHandle2 = LoadGraph("data/image/CrownParticle2.png");	// 王冠パーティクル2画像のロード
		_crownParticleHandle3 = LoadGraph("data/image/CrownParticle3.png");	// 王冠パーティクル3画像のロード
	}

	// 表示するパーティクルの初期設定
	_crownParticle = _crownParticleHandle1;
}

Crown::~Crown()
{
	DeleteGraph(_crownHandle);
	DeleteGraph(_crownParticleHandle1);
	DeleteGraph(_crownParticleHandle2);
	DeleteGraph(_crownParticleHandle3);
}

void Crown::Update()
{
	// 王冠全体を点滅させる
	if (!_flame1Flag) {
		if (_flame1 <= 0) {
			_flame1Flag = true;
			_blinkingFlag = false;
			_flame2 = 0;
		}
		else {
			_flame1 -= GetConstantInt("CROWN_SPEED");
		}
	}
	else {
		if (_flame1 >= Application::GetInstance().GetConstantInt("FRAME_NUM") * 2) {
			_blinkingFlag = true;
		}
		else {
			_flame1 += GetConstantInt("CROWN_SPEED");
		}
	}

	// パーティクルの点滅
	if (_blinkingFlag) {
		_flame2++;
		if (_flame2 <= Application::GetInstance().GetConstantInt("FRAME_NUM") / 2) {
			_crownParticle = _crownParticleHandle1;
		}
		else if (_flame2 <= Application::GetInstance().GetConstantInt("FRAME_NUM")) {
			_crownParticle = _crownParticleHandle2;
		}
		else if (_flame2 <= Application::GetInstance().GetConstantInt("FRAME_NUM") / 2 * 3) {
			_crownParticle = _crownParticleHandle3;
		}
		else {
			_flame1Flag = false;
		}
	}
}

void Crown::Draw()
{
	int alpha = static_cast<int>(255 * ((float)_flame1 / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// 王冠画像の描画
	DrawRotaGraph(_dispPos.intX(), _dispPos.intY(), GetConstantFloat("CROWN_EXRATE"), GetConstantFloat("CROWN_ANGLE"), _crownHandle, true);

	// 王冠パーティクル画像の描画
	if (_blinkingFlag) {
		DrawRotaGraph(_dispPos.intX(), _dispPos.intY(), GetConstantFloat("CROWN_EXRATE"), GetConstantFloat("CROWN_ANGLE"), _crownParticle, true);

	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
