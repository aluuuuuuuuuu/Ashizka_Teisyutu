#include "SelectFinger.h"
#include "DxLib.h"
#include "Application.h"
#include "Input.h"

SelectFinger::SelectFinger(int padNum) :
	_padNum(padNum),
	_chara(0),
	_selectFlag(false)
{
	// 初期一の設定
	_pos = Vec2{ Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2,
		Application::GetInstance().GetConstantInt("SCREEN_HEIGHT") / 2 };

	// 指画像のロード
	switch (padNum)
	{
	case 0:
		_fingerHandle = LoadGraph("data/image/finger_down_red.png");
		break;
	case 1:
		_fingerHandle = LoadGraph("data/image/finger_down_blue.png");
		break;
	case 2:
		_fingerHandle = LoadGraph("data/image/finger_down_green.png");
		break;
	case 3:
		_fingerHandle = LoadGraph("data/image/finger_down_yellow.png");
		break;
	default:
		_fingerHandle = 0;
		break;
	}
}

SelectFinger::~SelectFinger()
{
	// 画像の削除
	DeleteGraph(_fingerHandle);
}

void SelectFinger::Update()
{
	// 左スティックで移動
	Vec3 move;
	if (Input::GetInstance().GetStickVectorLength(INPUT_LEFT_STICK,_padNum) >= 5000) {
		move = Input::GetInstance().GetStickUnitVector(INPUT_LEFT_STICK, _padNum) * 10;
	}

	_pos += Vec2{ static_cast<int>(move.x),static_cast<int>(move.z * -1) };

	if (_pos.intX() >= 1920 - 100) {
		_pos.x = 1920 - 100;
	}
	else if (_pos.intX() <= 0 + 20) {
		_pos.x = 0 + 20;
	}

	if (_pos.intY() >= 1080 - 150) {
		_pos.y = 1080 - 150;
	}
	else if (_pos.intY() <= 0) {
		_pos.y = 0;
	}
}

void SelectFinger::Draw() const
{
	// 指の描画
	DrawGraph(_pos.intX() - 50, _pos.intY() - 30, _fingerHandle, true);
}


bool SelectFinger::IsPushButton(int button) const
{
	return  Input::GetInstance().IsTrigger(button, _padNum);
}

Vec2 SelectFinger::GetPos() const
{
	return _pos;
}

int SelectFinger::GetPadNum() const
{
	return _padNum;
}

void SelectFinger::SetSelectFlag(bool flag)
{
	_selectFlag = flag;
}

bool SelectFinger::IsSelected() const
{
	return _selectFlag;
}

void SelectFinger::SetSelectChara(int charaNum)
{
	_chara = charaNum;
}

int SelectFinger::GetSelectChara() const
{
	return _chara;
}
