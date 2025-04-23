#include "PlayerCamera.h"
#include "DxLib.h"
#include "Input.h"

PlayerCamera::PlayerCamera(Vec3 pos, int padNum, Vec3 forward):
	_lightHandle(0),
	_padNum(padNum)
{
	// 外部ファイルから定数を取得する
	ReadCSV("data/constant/PlayerCamera.csv");

	// カメラのニアファーの設定
	SetCameraNearFar(GetConstantFloat("CAMERA_NEAR"), GetConstantFloat("CAMERA_FAR"));

	Vec3 right = forward.Cross(Vec3{ 0,1,0 }).GetNormalized();

	Position = -forward * 10 + right + 10 + Vec3(0, 10, 0);

	// ライトの作成
	_lightHandle = CreateDirLightHandle(VECTOR{ 0,0,0 });
	SetLightDifColorHandle(_lightHandle, GetColorF(0.5f, 0.5f, 0.5f, 0.0f));
}

PlayerCamera::~PlayerCamera()
{
	DeleteLightHandle(_lightHandle);
}

void PlayerCamera::Update(Vec3 pos,Vec3 forward, Vec3 angle)
{
	// ターゲットの設定
	//_target = Vec3(pos.x, pos.y + GetConstantFloat("CAMERA_MARGIN_Y"), pos.z);
	
	_target = forward * 100 + VGet(pos.x,pos.y + 10,pos.z);

	Vec3 right = forward.Cross(Vec3{ 0,1,0 }).GetNormalized();

	Position = -forward * 20 + -right * 8 + Vec3(0, 15, 0) + pos;

	// 座標の設定
	//Position = Rotate(Position,forward);


	// ライトの角度を設定
	SetLightDirectionHandle(_lightHandle, (_target - Position).VGet());
}

Vec3 PlayerCamera::GetTarget()
{
	return _target;
}

Vec3 PlayerCamera::Rotate(Vec3 pos, Vec3 forward)
{
	// インプットのインスタンスを取得
	auto& input = Input::GetInstance();

	// 右スティックで回転
	if (input.GetStickVectorLength(INPUT_RIGHT_STICK, _padNum) > GetConstantFloat("STICK_INVALID_VALUE")) {

		// スティックを傾けた方向の回転の値を増減させる
		if (input.GetStickVector(INPUT_RIGHT_STICK, _padNum).x != 0) {
			Angle.y += GetConstantFloat("CAMERA_ANGLE_VARIATION") * (input.GetStickThumbX(INPUT_RIGHT_STICK, _padNum));
		}
		if (input.GetStickVector(INPUT_RIGHT_STICK, _padNum).z != 0) {
			Angle.z += GetConstantFloat("CAMERA_ANGLE_VARIATION") * (input.GetStickThumbY(INPUT_RIGHT_STICK, _padNum));
		}

		// ラジアン角を正規化する
		Angle.y = fmodf(Angle.y, static_cast<float>(DX_TWO_PI));
		if (Angle.y < 0.0f) Angle.y += static_cast<float>(DX_TWO_PI);
	}

	// 最大値と最低値を調整する
	if (Angle.z <= -GetConstantFloat("CAMERA_ANGLE_RANGE")) {
		Angle.z = -GetConstantFloat("CAMERA_ANGLE_RANGE");
	}
	else if (Angle.z >= GetConstantFloat("CAMERA_ANGLE_RANGE")) {
		Angle.z = GetConstantFloat("CAMERA_ANGLE_RANGE");
	}

	return RotatePos(pos);



	// 基準となるカメラの座標
	//Vec3 basePos = VGet(GetConstantFloat("CAMERA_BASE_POS_X"),GetConstantFloat("CAMERA_BASE_POS_Y"), GetConstantFloat("CAMERA_BASE_POS_Z"));

	// カメラ座標はプレイヤー座標から変換した座標を足したところ
	//return VAdd(VGet(pos.x, pos.y + GetConstantFloat("CAMERA_BASE_POS_Y"), pos.z), RotatePos(basePos).VGet());
}

Vec3 PlayerCamera::RotatePos(Vec3 pos)
{
	// 回転行列を作成
	MATRIX rotMtxX, rotMtxZ;
	rotMtxX = MGetRotY(Angle.y);
	rotMtxZ = MGetRotX(Angle.z);

	// 基準座標を行列で変換
	pos = VTransform(pos.VGet(), rotMtxZ);
	pos = VTransform(pos.VGet(), rotMtxX);

	return pos;
}
