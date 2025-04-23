#include "SelectUI.h"
#include "DxLib.h"

SelectUI::SelectUI()
{
	// 文章画像のロード
	_selectFighterHandle = LoadGraph("data/image/SelectFighter.png");

	// 戻る画像のロード
	_returnHandle = LoadGraph("data/image/return.png");
}

SelectUI::~SelectUI()
{
}

void SelectUI::Update()
{
}

void SelectUI::Draw() const
{
	// 戻る画像の描画
	DrawGraph(0, 0, _returnHandle, true);

	// 文章の描画
	DrawRotaGraph(960, 190, 1, 0, _selectFighterHandle, true);
}
