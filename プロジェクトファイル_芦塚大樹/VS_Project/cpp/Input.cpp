#include "Input.h"
#include <cassert>

void Input::Update()
{
	// 接続されているコントローラーの数を取得する
	_padNum = GetJoypadNum();

	int num = 0x0001;

	// 以前のステートを保存
	for (int i = 0; i < _padNum; i++) {

		// 以前のステートを保存
		_lastPadState[i] = _padState[i];

		// 現在のステートを取得
		GetJoypadXInputState(num, &_padState[i]);

		num += 0x0001;
	}
}

// 押した瞬間
bool Input::IsTrigger(int input, int padNum) const
{
	assert(padNum < _padNum);

	if (input == INPUT_A) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_A] && _padState[padNum].Buttons[XINPUT_BUTTON_A]);
	}
	if (input == INPUT_B) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_B] && _padState[padNum].Buttons[XINPUT_BUTTON_B]);
	}
	if (input == INPUT_X) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_X] && _padState[padNum].Buttons[XINPUT_BUTTON_X]);
	}
	if (input == INPUT_Y) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_Y] && _padState[padNum].Buttons[XINPUT_BUTTON_Y]);
	}
	if (input == INPUT_UP) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_UP] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_UP]);
	}
	if (input == INPUT_DOWN) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_DOWN] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_DOWN]);
	}
	if (input == INPUT_LEFT) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_LEFT] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_LEFT]);
	}
	if (input == INPUT_RIGHT) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_RIGHT] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_RIGHT]);
	}
	if (input == INPUT_START) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_START] && _padState[padNum].Buttons[XINPUT_BUTTON_START]);
	}
	if (input == INPUT_BACK) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_BACK] && _padState[padNum].Buttons[XINPUT_BUTTON_BACK]);
	}
	if (input == INPUT_LEFT_PUSH) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_LEFT_THUMB] && _padState[padNum].Buttons[XINPUT_BUTTON_LEFT_THUMB]);
	}
	if (input == INPUT_RIGHT_PUSH) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_RIGHT_THUMB] && _padState[padNum].Buttons[XINPUT_BUTTON_RIGHT_THUMB]);
	}
	if (input == INPUT_LEFT_SHOULDER) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_LEFT_SHOULDER] && _padState[padNum].Buttons[XINPUT_BUTTON_LEFT_SHOULDER]);
	}
	if (input == INPUT_RIGHT_SHOULDER) {
		return (!_lastPadState[padNum].Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] && _padState[padNum].Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]);
	}
	if (input == INPUT_LEFT_TRIGGER) {
		return (_lastPadState[padNum].LeftTrigger <= 0 && _padState[padNum].LeftTrigger >= 1);
	}
	if (input == INPUT_RIGHT_TRIGGER) {
		return (_lastPadState[padNum].RightTrigger <= 0 && _padState[padNum].RightTrigger >= 1);
	}

	return false;
}

// 押している間
bool Input::IsHold(int input, int padNum) const
{
	if (input == INPUT_A) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_A] && _padState[padNum].Buttons[XINPUT_BUTTON_A]);
	}
	if (input == INPUT_B) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_B] && _padState[padNum].Buttons[XINPUT_BUTTON_B]);
	}
	if (input == INPUT_X) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_X] && _padState[padNum].Buttons[XINPUT_BUTTON_X]);
	}
	if (input == INPUT_Y) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_Y] && _padState[padNum].Buttons[XINPUT_BUTTON_Y]);
	}
	if (input == INPUT_UP) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_UP] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_UP]);
	}
	if (input == INPUT_DOWN) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_DOWN] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_DOWN]);
	}
	if (input == INPUT_LEFT) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_LEFT] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_LEFT]);
	}
	if (input == INPUT_RIGHT) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_RIGHT] && _padState[padNum].Buttons[XINPUT_BUTTON_DPAD_RIGHT]);
	}
	if (input == INPUT_START) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_START] && _padState[padNum].Buttons[XINPUT_BUTTON_START]);
	}
	if (input == INPUT_BACK) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_BACK] && _padState[padNum].Buttons[XINPUT_BUTTON_BACK]);
	}
	if (input == INPUT_LEFT_PUSH) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_LEFT_THUMB] && _padState[padNum].Buttons[XINPUT_BUTTON_LEFT_THUMB]);
	}
	if (input == INPUT_RIGHT_PUSH) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_RIGHT_THUMB] && _padState[padNum].Buttons[XINPUT_BUTTON_RIGHT_THUMB]);
	}
	if (input == INPUT_LEFT_SHOULDER) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_LEFT_SHOULDER] && _padState[padNum].Buttons[XINPUT_BUTTON_LEFT_SHOULDER]);
	}
	if (input == INPUT_RIGHT_SHOULDER) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] && _padState[padNum].Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]);
	}
	if (input == INPUT_LEFT_TRIGGER) {
		return (_lastPadState[padNum].LeftTrigger >= 1 && _padState[padNum].LeftTrigger >= 1);
	}
	if (input == INPUT_RIGHT_TRIGGER) {
		return (_lastPadState[padNum].RightTrigger >= 1 && _padState[padNum].RightTrigger >= 1);
	}
	return false;
}

// 離した瞬間
bool Input::IsRelease(int input, int padNum) const
{
	if (input == INPUT_A) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_A] && !_padState[padNum].Buttons[XINPUT_BUTTON_A]);
	}
	if (input == INPUT_B) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_B] && !_padState[padNum].Buttons[XINPUT_BUTTON_B]);
	}
	if (input == INPUT_X) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_X] && !_padState[padNum].Buttons[XINPUT_BUTTON_X]);
	}
	if (input == INPUT_Y) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_Y] && !_padState[padNum].Buttons[XINPUT_BUTTON_Y]);
	}
	if (input == INPUT_UP) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_UP] && !_padState[padNum].Buttons[XINPUT_BUTTON_DPAD_UP]);
	}
	if (input == INPUT_DOWN) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_DOWN] && !_padState[padNum].Buttons[XINPUT_BUTTON_DPAD_DOWN]);
	}
	if (input == INPUT_LEFT) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_LEFT] && !_padState[padNum].Buttons[XINPUT_BUTTON_DPAD_LEFT]);
	}
	if (input == INPUT_RIGHT) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_DPAD_RIGHT] && !_padState[padNum].Buttons[XINPUT_BUTTON_DPAD_RIGHT]);
	}
	if (input == INPUT_START) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_START] && !_padState[padNum].Buttons[XINPUT_BUTTON_START]);
	}
	if (input == INPUT_BACK) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_BACK] && !_padState[padNum].Buttons[XINPUT_BUTTON_BACK]);
	}
	if (input == INPUT_LEFT_PUSH) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_LEFT_THUMB] && !_padState[padNum].Buttons[XINPUT_BUTTON_LEFT_THUMB]);
	}
	if (input == INPUT_RIGHT_PUSH) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_RIGHT_THUMB] && !_padState[padNum].Buttons[XINPUT_BUTTON_RIGHT_THUMB]);
	}
	if (input == INPUT_LEFT_SHOULDER) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_LEFT_SHOULDER] && !_padState[padNum].Buttons[XINPUT_BUTTON_LEFT_SHOULDER]);
	}
	if (input == INPUT_RIGHT_SHOULDER) {
		return (_lastPadState[padNum].Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] && !_padState[padNum].Buttons[XINPUT_BUTTON_RIGHT_SHOULDER]);
	}
	if (input == INPUT_LEFT_TRIGGER) {
		return (_lastPadState[padNum].LeftTrigger >= 1 && _padState[padNum].LeftTrigger <= 0);
	}
	if (input == INPUT_RIGHT_TRIGGER) {
		return (_lastPadState[padNum].RightTrigger >= 1 && _padState[padNum].RightTrigger <= 0);
	}
	return false;
}

bool Input::AnyPressButton(int padNum) const
{
	for (auto& button : _padState[padNum].Buttons) {
		if (button == 1) {
			return true;
		}
	}
	return false;
}

Vec3 Input::GetStickVector(int input, int padNum) const
{
	if (input == INPUT_LEFT_STICK) {
		Vec3 vec = { static_cast<float>(_padState[padNum].ThumbLX),0.0f,static_cast<float>(_padState[padNum].ThumbLY) };
		return vec;
	}
	if (input == INPUT_RIGHT_STICK) {
		Vec3 vec = { static_cast<float>(_padState[padNum].ThumbRX),0.0f,static_cast<float>(_padState[padNum].ThumbRY) };
		return vec;
	}
	return Vec3();
}

Vec3 Input::GetStickUnitVector(int input, int padNum) const
{
	if (input == INPUT_LEFT_STICK) {
		Vec3 vec = { static_cast<float>(_padState[padNum].ThumbLX),0.0f,static_cast<float>(_padState[padNum].ThumbLY) };
		return vec.GetNormalized();
	}
	if (input == INPUT_RIGHT_STICK) {
		Vec3 vec = { static_cast<float>(_padState[padNum].ThumbRX),0.0f,static_cast<float>(_padState[padNum].ThumbRY) };
		return vec.GetNormalized();
	}
	return Vec3();
}

float Input::GetStickVectorLength(int input, int padNum) const
{
	if (input == INPUT_LEFT_STICK) {
		Vec3 vec = { static_cast<float>(_padState[padNum].ThumbLX),0.0f,static_cast<float>(_padState[padNum].ThumbLY) };
		return vec.Length();
	}
	if (input == INPUT_RIGHT_STICK) {
		Vec3 vec = { static_cast<float>(_padState[padNum].ThumbRX),0.0f,static_cast<float>(_padState[padNum].ThumbRY) };
		return vec.Length();
	}
	return 0.0f;
}

float Input::GetStickThumbX(int input, int padNum) const
{
	if (input == INPUT_LEFT_STICK) {
		return static_cast<float>(_padState[padNum].ThumbLX);
	}
	if (input == INPUT_RIGHT_STICK) {
		return static_cast<float>(_padState[padNum].ThumbRX);
	}
	return 0.0f;
}

float Input::GetStickThumbY(int input, int padNum) const
{
	if (input == INPUT_LEFT_STICK) {
		return static_cast<float>(_padState[padNum].ThumbLY);
	}
	if (input == INPUT_RIGHT_STICK) {
		return static_cast<float>(_padState[padNum].ThumbRY);
	}
	return 0.0f;
}

int Input::GetPadNum()
{
	return _padNum;
}
