#pragma once
#include <array>
#include "DxLib.h"
#include "Singleton.h"
#include "Vec3.h"

// 各パッド
#define INPUT_PAD_1 0
#define INPUT_PAD_2 1
#define INPUT_PAD_3 2
#define INPUT_PAD_4 3

// 各ボタンの定数
#define INPUT_A 0
#define INPUT_B 1
#define INPUT_X 2
#define INPUT_Y 3
#define INPUT_UP 4
#define INPUT_DOWN 5
#define INPUT_LEFT 6
#define INPUT_RIGHT 7
#define INPUT_START 8
#define INPUT_BACK 9
#define INPUT_LEFT_PUSH 10
#define INPUT_RIGHT_PUSH 11
#define INPUT_LEFT_SHOULDER 12
#define INPUT_RIGHT_SHOULDER 13
#define INPUT_LEFT_TRIGGER 14
#define INPUT_RIGHT_TRIGGER 15
#define INPUT_LEFT_STICK 16
#define INPUT_RIGHT_STICK 17

// DXライブラリで提供される構造体の中身
// struct XINPUT_STATE
// {
// 	 unsigned char	Buttons[16];	// ボタン１６個( 添字には XINPUT_BUTTON_DPAD_UP 等を使用する、
// 	 			0:押されていない  1:押されている )
// 	 unsigned char	LeftTrigger;	// 左トリガー( 0～255 )
//	 unsigned char	RightTrigger;	// 右トリガー( 0～255 )
//	 short		ThumbLX;	// 左スティックの横軸値( -32768 ～ 32767 )
//	 short		ThumbLY;	// 左スティックの縦軸値( -32768 ～ 32767 )
//	 short		ThumbRX;	// 右スティックの横軸値( -32768 ～ 32767 )
//	 short		ThumbRY;	// 右スティックの縦軸値( -32768 ～ 32767 )
// };

// インプット情報を提供するシングルトンクラス
class Input : public Singleton<Input>
{
	friend class Singleton<Input>;

public:
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 押した瞬間をとる
	/// </summary>
	/// <param name="input">ボタン</param>
	/// <param name="padNum">パッドナンバー</param>
	/// <returns>押した瞬間true</returns>
	bool IsTrigger(int input, int padNum) const;
	

	/// <summary>
	/// 押している間をとる
	/// </summary>
	/// <param name="input">ボタン</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>押し続けている間true</returns>
	bool IsHold(int input, int padNum) const;

	/// <summary>
	/// 離した瞬間をとる
	/// </summary>
	/// <param name="input">ボタン</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>離した瞬間true</returns>
	bool IsRelease(int input, int padNum) const;

	/// <summary>
	/// いずれかのボタンが押されているかどうか返す
	/// </summary>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>何かが押されているかどうか</returns>
	bool AnyPressButton(int padNum) const;

	/// <summary>
	/// スティック入力のベクトルを返す
	/// </summary>
	/// <param name="input">スティック</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>スティックを傾けている方向のベクトル</returns>
	Vec3 GetStickVector(int input, int padNum) const;

	/// <summary>
	/// スティック入力の単位ベクトルを返す
	/// </summary>
	/// <param name="input">スティック</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>スティックを傾けている方向の単位ベクトル</returns>
	Vec3 GetStickUnitVector(int input, int padNum) const;

	/// <summary>
	/// スティックの傾き具合を返す
	/// </summary>
	/// <param name="input">スティック</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>スティック入力のベクトルの長さ</returns>
	float GetStickVectorLength(int input, int padNum) const;

	/// <summary>
	/// スティックを傾けたX値を返す
	/// </summary>
	/// <param name="input">スティック</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>スティックを傾けてるベクトルのXの値</returns>
	float GetStickThumbX(int input, int padNum) const;

	/// <summary>
	/// スティックを傾けたY値を返す
	/// </summary>
	/// <param name="input">スティック</param>
	/// <param name="padNum">パッドナンバー<</param>
	/// <returns>スティックを傾けてるベクトルのYの値</returns>
	float GetStickThumbY(int input, int padNum) const;

	/// <summary>
	/// 接続されているコントローラーの数を取得する
	/// </summary>
	/// <returns>コントローラーの数</returns>
	int GetPadNum();

private:
	// 今フレームのインプットステート
	XINPUT_STATE _padState[4] = {};

	// 前フレームのインプットステート
	XINPUT_STATE _lastPadState[4] = {};

	// プライベートコンストラクタ
	Input() {};

	// 接続されているコントローラーの数
	int _padNum = 0;
};

