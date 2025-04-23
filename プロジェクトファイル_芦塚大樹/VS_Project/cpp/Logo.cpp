#include "Logo.h"
#include "DxLib.h"
#include "SceneTitle.h"

Logo::Logo(SceneTitle& scene)
{
	// 画像のロード
	_logo = LoadGraph("data/image/TitleLogo.png");

	// 表示位置の設定
	_dispPos = Vec2{ scene.GetConstantInt("LOGO_POS_X"),scene.GetConstantInt("LOGO_POS_Y") };
}

Logo::~Logo()
{
	DeleteGraph(_logo);
}

void Logo::Draw() const
{
	// ロゴの描画
	DrawRotaGraph(_dispPos.intX(), _dispPos.intY(), 1.0f, 0.0f, _logo, true);
}
