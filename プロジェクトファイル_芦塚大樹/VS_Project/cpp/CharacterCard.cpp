#include "CharacterCard.h"
#include "DxLib.h"
#include "SceneSelect.h"
#include "PlayerManager.h"

CharacterCard::CharacterCard(int charNum) :
	_cardHandle(0),
	_characterNum(charNum)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/CharacterCard.csv");

	// キャラ画像のロード
	switch (charNum)
	{
	case 0:
		_cardHandle = LoadGraph("data/image/Card1.png");
		_pos = Vec2{ GetConstantInt("CARD1_POS_X"),GetConstantInt("CARD1_POS_Y") };
		break;
	case 1:
		_cardHandle = LoadGraph("data/image/Card2.png");
		_pos = Vec2{ GetConstantInt("CARD2_POS_X"),GetConstantInt("CARD2_POS_Y") };
		break;
	case 2:
		_cardHandle = LoadGraph("data/image/Card3.png");
		_pos = Vec2{ GetConstantInt("CARD3_POS_X"),GetConstantInt("CARD3_POS_Y") };
		break;
	case 3:
		_cardHandle = LoadGraph("data/image/Card4.png");
		_pos = Vec2{ GetConstantInt("CARD4_POS_X"),GetConstantInt("CARD4_POS_Y") };
		break;
	default:
		break;
	}

	// 選択後画像のロード
	{
		_cardSelectHandle[PLAYER_ONE] = LoadGraph("data/image/CardSelectedRed.png");
		_cardSelectHandle[PLAYER_TWO] = LoadGraph("data/image/CardSelectedBlue.png");
		_cardSelectHandle[PLAYER_THREE] = LoadGraph("data/image/CardSelectedGreen.png");
		_cardSelectHandle[PLAYER_FORE] = LoadGraph("data/image/CardSelectedYellow.png");
	}

	// 選択フラグの初期化
	for (auto& flag : _selectFlag) {
		flag = false;
	}
}

CharacterCard::~CharacterCard()
{
	// 画像のデリート
	DeleteGraph(_cardHandle);
	for (auto& handle : _cardSelectHandle) {
		DeleteGraph(handle);
	}
}

void CharacterCard::Update(std::shared_ptr<SelectFinger>& fing)
{
}

void CharacterCard::Draw() const
{
	// ガードの描画
	DrawGraph(_pos.intX(), _pos.intY(), _cardHandle, true);

	// フラグに応じて選択後の画像を描画する
	for (int i = 0; i <= PLAYER_FORE; i++) {
		if (_selectFlag[i]) {
			DrawGraph(_pos.intX() + 10, _pos.intY() + 10, _cardSelectHandle[i], true);
			break;
		}
	}
}

void CharacterCard::SelectCharacter(int plNum)
{
	_selectFlag[plNum] = true;
}

void CharacterCard::RejectCharacter(int plNum)
{
	_selectFlag[plNum] = false;
}

bool CharacterCard::GetSelectFlag()
{
	for (auto& flag : _selectFlag) {
		if (flag) return true;
	}
	return false;
}

bool CharacterCard::GetSelectFlag(int plNum)
{
	return _selectFlag[plNum];
}

Vec2 CharacterCard::GetPos() const
{
	return _pos;
}

int CharacterCard::GetCharacterNum() const
{
	return _characterNum;
}
