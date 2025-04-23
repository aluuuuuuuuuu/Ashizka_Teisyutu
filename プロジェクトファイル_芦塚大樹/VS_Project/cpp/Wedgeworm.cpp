#include "Wedgeworm.h"
#include "DxLib.h"

Wedgeworm::Wedgeworm(int num,int model):
	_angle(0)
{
	// 初期座標の設定
	_pos = Vec3{ 120.0f,50.0f,120.0f };

	// ナンバーで初期座標を切り替える
	if (num == 0) {
		_angle = 3.14f;
		Angle.y = 0.0f;
	}
	else {
		_angle = 0.0f;
		Angle.y = 3.14f;
	}
	
	// 回転行列を初期角度で回転させておく
	MATRIX mtx = MGetRotY(_angle);
	_pos = VTransform(_pos.VGet(), mtx);

	// 拡大の設定
	Scale = Vec3{ 0.1f,0.1f,0.1f };

	// モデルの初期処理
	InitModel(MV1DuplicateModel(model));

	// 常に回転する量を入力
	_angle = 0.005f;

	// 回転行列を作成する
	_mtx = MGetRotY(_angle);
}

Wedgeworm::~Wedgeworm()
{
}

void Wedgeworm::Update()
{

	// 作成した回転行列を座標に適用する
	_pos = VTransform(_pos.VGet(), _mtx);

	Angle.y += 0.0045f;

	// ラジアン角を正規化する
	Angle.y = fmodf(Angle.y, static_cast<float>(DX_TWO_PI));
	if (Angle.y < 0.0f) Angle.y += static_cast<float>(DX_TWO_PI);

	// 新しく作成して座標を調整する
	Position = Vec3{ _pos.x + 120, _pos.y,_pos.z + 120 };

	// モデルの更新処理
	UpdateModel(GetTransformInstance());
}

void Wedgeworm::Draw() const
{
	// モデルの描画
	DrawModel();
}
