#include "CreditCharacter.h"
#include "DxLib.h"

CreditCharacter::CreditCharacter(int num) :
	_num(num)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/CreditCharactor.csv");

	// 拡大の設定
	Scale = GetConstantFloat("CHARACTOR_SCALE");

	// 回転の設定
	Angle = Vec3{ GetConstantFloat("CHARACTOR_ANGLE_X"),
		GetConstantFloat("CHARACTOR_ANGLE_Y"),
		GetConstantFloat("CHARACTOR_ANGLE_Z") };

	// プレイヤーナムに合わせてキャラをロードする
	switch (num)
	{
	case 0:
		InitModel(MV1LoadModel("data/model/DancePlayer1.mv1"));

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y"), GetConstantFloat("CHARACTOR_POS_Z") };
		break;
	case 1:
		InitModel(MV1LoadModel("data/model/DancePlayer2.mv1"));

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y"),  -GetConstantFloat("CHARACTOR_POS_Z") };
		break;
	case 2:
		InitModel(MV1LoadModel("data/model/DancePlayer3.mv1"));

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y"), GetConstantFloat("CHARACTOR_POS_Z") };
		break;
	case 3:
		InitModel(MV1LoadModel("data/model/DancePlayer4.mv1"));

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y"), -GetConstantFloat("CHARACTOR_POS_Z") };
		break;
	default:
		break;
	}

	InitAnimation(_modelHandle, GetConstantInt("ANIM_IDLE"), GetConstantFloat("ANIM_BLEND_RATE"));
	ChangeAnimation(_modelHandle, GetConstantInt("ANIM_DANCE"), true, GetConstantFloat("ANIM_BLEND_RATE"));


	// モデルの更新
	UpdateModel(GetTransformInstance());
}

CreditCharacter::~CreditCharacter()
{
}

void CreditCharacter::Update()
{
	// アニメーションの更新
	UpdateAnimation(_modelHandle, GetConstantFloat("ANIM_SPEED"));

	// モデルの更新
	UpdateModel(GetTransformInstance());
}

void CreditCharacter::Draw() const
{
	DrawModel();
}

void CreditCharacter::PosSet()
{
	switch (_num)
	{
	case 0:

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y"), GetConstantFloat("CHARACTOR_POS_Z") };
		break;
	case 1:

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y") + 10.0f,  -GetConstantFloat("CHARACTOR_POS_Z") };
		break;
	case 2:

		// 座標の設定
		Position = Vec3{ GetConstantFloat("CHARACTOR_POS_X"), GetConstantFloat("CHARACTOR_POS_Y") + 10.0f, GetConstantFloat("CHARACTOR_POS_Z") + 5.0f };
		break;
	case 3:
		break;
	default:
		break;
	}
}
