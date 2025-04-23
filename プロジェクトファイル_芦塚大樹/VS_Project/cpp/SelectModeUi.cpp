#include "SelectModeUi.h"
#include "DxLib.h"
#include "Input.h"
#include "MyMath.h"
#include "Vec3.h"
#include "SoundManager.h"

SelectModeUi::SelectModeUi(bool selectMode) :
	_selectButtonNum(SOLO_MODE),
	_circleAngle(0.0),
	_frame(0),
	_input(false),
	_circleScale(1.0),
	_changeFrag(false)
{
	// 画像のロード
	{
		// 中央の円
		_circleHandle[SOLO_MODE] = LoadGraph("data/image/CenterCircle1.png");
		_circleHandle[MULTI_MODE] = LoadGraph("data/image/CenterCircle2.png");
		_circleHandle[TUTORIAL_MODE] = LoadGraph("data/image/CenterCircle3.png");
		_circleHandle[OPTION_MODE] = LoadGraph("data/image/CenterCircle4.png");

		_circleLogoHandle = LoadGraph("data/image/CenterCircleLogo.png");

		_centerArrowHandle[SOLO_MODE] = LoadGraph("data/image/CenterArrow1.png");
		_centerArrowHandle[MULTI_MODE] = LoadGraph("data/image/CenterArrow2.png");
		_centerArrowHandle[TUTORIAL_MODE] = LoadGraph("data/image/CenterArrow3.png");
		_centerArrowHandle[OPTION_MODE] = LoadGraph("data/image/CenterArrow4.png");

		// ボタン
		if (selectMode) {
			_buttonHandle[SOLO_MODE] = LoadGraph("data/image/Button1.png");
			_buttonHandle[MULTI_MODE] = LoadGraph("data/image/Button2.png");
			_buttonHandle[TUTORIAL_MODE] = LoadGraph("data/image/Button3.png");
			_buttonHandle[OPTION_MODE] = LoadGraph("data/image/Button4.png");
		}
		else {
			_buttonHandle[SOLO_MODE] = LoadGraph("data/image/PlayerNum1.png");
			_buttonHandle[MULTI_MODE] = LoadGraph("data/image/PlayerNum2.png");
			_buttonHandle[TUTORIAL_MODE] = LoadGraph("data/image/PlayerNum3.png");
			_buttonHandle[OPTION_MODE] = LoadGraph("data/image/PlayerNum4.png");
		}

		// フレーム
		_frameHandle[SOLO_MODE] = LoadGraph("data/image/Frame1.png");
		_frameHandle[MULTI_MODE] = LoadGraph("data/image/Frame2.png");
		_frameHandle[TUTORIAL_MODE] = LoadGraph("data/image/Frame3.png");
		_frameHandle[OPTION_MODE] = LoadGraph("data/image/Frame4.png");

		// ハイライト
		_highlightHandle[SOLO_MODE] = LoadGraph("data/image/Highlight1.png");
		_highlightHandle[MULTI_MODE] = LoadGraph("data/image/Highlight2.png");
		_highlightHandle[TUTORIAL_MODE] = LoadGraph("data/image/Highlight3.png");
		_highlightHandle[OPTION_MODE] = LoadGraph("data/image/Highlight4.png");

		// メッセージ
		if (selectMode) {
			_messageHandle[SOLO_MODE] = LoadGraph("data/image/Message01.png");
			_messageHandle[MULTI_MODE] = LoadGraph("data/image/Message02.png");
			_messageHandle[TUTORIAL_MODE] = LoadGraph("data/image/Message03.png");
			_messageHandle[OPTION_MODE] = LoadGraph("data/image/Message04.png");
		}
		else {
			_messageHandle[SOLO_MODE] = LoadGraph("data/image/Play1.png");
			_messageHandle[MULTI_MODE] = LoadGraph("data/image/Play2.png");
			_messageHandle[TUTORIAL_MODE] = LoadGraph("data/image/Play3.png");
			_messageHandle[OPTION_MODE] = LoadGraph("data/image/Play4.png");
		}
		

		// 余白
		_marginHandle[SOLO_MODE] = LoadGraph("data/image/Margin1.png");
		_marginHandle[MULTI_MODE] = LoadGraph("data/image/Margin2.png");
		_marginHandle[TUTORIAL_MODE] = LoadGraph("data/image/Margin3.png");
		_marginHandle[OPTION_MODE] = LoadGraph("data/image/Margin4.png");
	}

	// ボタン画像の座標を設定する
	_buttonPos[SOLO_MODE] = Vec2{ 202, 118 };
	_buttonPos[MULTI_MODE] = Vec2{ 976, 118 };
	_buttonPos[TUTORIAL_MODE] = Vec2{ 202, 631 };
	_buttonPos[OPTION_MODE] = Vec2{ 976, 631 };

	// 中央の円の座標
	_circlePos = Vec2{ 960,620 };

	// 最初に選択されている項目の設定
	_selectButtonNum = TUTORIAL_MODE;
	_oldSelectButtonNum = _selectButtonNum;
}

SelectModeUi::~SelectModeUi()
{
	// 画像の削除
	for (int i = 0; i < OPTION_MODE; i++) {
		DeleteGraph(_circleHandle[i]);
		DeleteGraph(_centerArrowHandle[i]);
		DeleteGraph(_buttonHandle[i]);
		DeleteGraph(_highlightHandle[i]);
		DeleteGraph(_messageHandle[i]);
		DeleteGraph(_marginHandle[i]);
	}

	DeleteGraph(_circleLogoHandle);
}

void SelectModeUi::Update()
{
	// スティックで選択を切り替える
	Vec3 stick = Input::GetInstance().GetStickVector(INPUT_LEFT_STICK, INPUT_PAD_1);

	if (Input::GetInstance().GetStickVectorLength(INPUT_LEFT_STICK, INPUT_PAD_1) > 10000.0f) {

		if (stick.x < 0.0f && stick.z > 0.0f) {
			_selectButtonNum = SOLO_MODE;
		}
		else if (stick.x > 0.0f && stick.z > 0.0f) {
			_selectButtonNum = MULTI_MODE;
		}
		else if (stick.x < 0.0f && stick.z < 0.0f) {
			_selectButtonNum = TUTORIAL_MODE;
		}
		else if (stick.x > 0.0f && stick.z < 0.0f) {
			_selectButtonNum = OPTION_MODE;
		}

		// フレームを初期化
		if (_oldSelectButtonNum != _selectButtonNum) {
			_circleScale = 0.7;
			_frame = 0;
			SoundManager::GetInstance().RingSE(SE_NUM_SELECT);
		}

		// 変更フラグを立てる
		_changeFrag = true;
	}

	// 選択しているボタンを保存する
	_oldSelectButtonNum = _selectButtonNum;

	// 円の拡大率を変更する
	_circleScale += 0.05;
	_circleScale = min(1.0, _circleScale);


	// 選択されているボタンによって角度を変える
	switch (_selectButtonNum)
	{
	case SOLO_MODE:
		_circleAngle = GetRadian(SOLO_ANGLE);
		_arrowVec = Vec2{ -1.0f, -1.0f }.GetNormalized();
		break;
	case MULTI_MODE:
		_circleAngle = GetRadian(MULTI_ANGLE);
		_arrowVec = Vec2{ 1.0f, -1.0f }.GetNormalized();
		break;
	case TUTORIAL_MODE:
		_circleAngle = GetRadian(TUTORIAL_ANGLE);
		_arrowVec = Vec2{ -1.0f, 1.0f }.GetNormalized();
		break;
	case OPTION_MODE:
		_circleAngle = GetRadian(OPTION_ANGLE);
		_arrowVec = Vec2{ -1.0f, -1.0f }.GetNormalized();
		break;
	default:
		break;
	}

	// 円の拡大率が１であれば矢印を動かす
	if (_circleScale == 1.0) {
		_frame++;

		float frequency = 2.0f * DX_PI_F / 60;
		_scale = 5.0f + 20.0f * std::sin(frequency * _frame);
		_arrowVec = _arrowVec * _scale;
	}
}

void SelectModeUi::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
	DrawBox(0, 0, 1980, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ボタンのハイライトの描画
	DrawGraph(_buttonPos[_selectButtonNum].intX(), _buttonPos[_selectButtonNum].intY(), _highlightHandle[_selectButtonNum], true);

	// ボタンの描画
	for (int i = 0; i < 4; i++) {
		//DrawRotaGraph(_buttonPos[i].intX(), _buttonPos[i].intY(), 1.0f, 0.0, _buttonHandle[i], true);
		DrawGraph(_buttonPos[i].intX(), _buttonPos[i].intY(), _buttonHandle[i], true);
	}


	// 中央の円の描画
	DrawRotaGraph(_circlePos.intX(), _circlePos.intY(), 1.0, 0.0, _marginHandle[_selectButtonNum], true);

	// フレームの描画
	DrawGraph(_buttonPos[_selectButtonNum].intX(), _buttonPos[_selectButtonNum].intY(), _frameHandle[_selectButtonNum], true);
	DrawRotaGraph(_circlePos.intX(), _circlePos.intY(), _circleScale, 0.0, _circleHandle[_selectButtonNum], true);
	DrawRotaGraph(_circlePos.intX(), _circlePos.intY(), _circleScale, 0.0, _circleLogoHandle, true);
	DrawRotaGraph(_circlePos.intX() + _arrowVec.intX(), _circlePos.intY() + _arrowVec.intY(), _circleScale, _circleAngle, _centerArrowHandle[_selectButtonNum], true);

	// 下の四角の描画
	DrawBox(0, 986, 1920, 1080, 0x000000, true);

	// 下のメッセージハンドル
	DrawRotaGraph(960, 1033, 1.0, 0.0, _messageHandle[_selectButtonNum], true);
}

int SelectModeUi::GetSelect() const
{
	return _selectButtonNum;
}
