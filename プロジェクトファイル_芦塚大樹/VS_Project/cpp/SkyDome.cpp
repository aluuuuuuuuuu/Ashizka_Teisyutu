#include "SkyDome.h"
#include "DxLib.h"

SkyDome::SkyDome()
{
	// モデルの初期処理
	InitModel(MV1LoadModel("data/model/Dome.mv1"));


	// 拡大率の設定
	Scale = 7.0f;

	// 座標の設定
	Position = Vec3{ 100.0f,100.0f,0.0f };

	// 一度上の設定を反映させる
	UpdateModel(GetTransformInstance());
}

SkyDome::~SkyDome()
{
}

void SkyDome::Update()
{
	// 少しづつドームをy軸させて雲が動いてるふうに見せる
	Angle.y += 0.0005f;

	// モデルの更新処理
	UpdateModel(GetTransformInstance());
}

void SkyDome::Draw() const
{
	// 描画処理
	DrawModel();
}
