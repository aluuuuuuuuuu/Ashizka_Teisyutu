#include "SceneSelectNum.h"
#include "SelectModeUi.h"
#include "SkyDome.h"
#include "SoundManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneSelectMode.h"
#include "PlayerManager.h"
#include "SceneSelect.h"
#include "Application.h"

SceneSelectNum::SceneSelectNum() :
	_selectFrag(false),
	_warningFrag(false),
	_returnFrag(false)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/SceneSelectNum.csv");

	// 関数ポインタの初期化
	_updateFunc = &SceneSelectNum::SlideInUpdate;
	_drawFunc = &SceneSelectNum::SlideInDraw;

	// Uiインスタンス作成
	_pUi = std::make_shared<SelectModeUi>(false);

	// スカイドームインスタンス作成
	_pSkyDome = std::make_shared<SkyDome>();

	// スライド画像のロード
	_slideHandle = LoadGraph("data/image/Slide.png");

	_slidePos.x = GetConstantInt("SLIDE_OUT_START_X");

	// カメラの初期化
	SetCameraPositionAndTarget_UpVecY(VECTOR{ GetConstantFloat("CAMERA_POS_X"),GetConstantFloat("CAMERA_POS_Y"),GetConstantFloat("CAMERA_POS_Z") },
		VECTOR{ GetConstantFloat("CAMERA_TARGET_X"), GetConstantFloat("CAMERA_TARGET_Y"), GetConstantFloat("CAMERA_TARGET_Z") });

	SetCameraNearFar(GetConstantFloat("CAMERA_NEAR"), GetConstantFloat("CAMERA_FAR"));

	// warning画像のロード
	_warningHandle = LoadGraph("data/image/warning.png");

	// BGMの再生
	SoundManager::GetInstance().StartBGM(BGM_OPENING);
}

SceneSelectNum::~SceneSelectNum()
{
	DeleteGraph(_slideHandle);
	DeleteGraph(_warningHandle);
}

void SceneSelectNum::Update()
{
	(this->*_updateFunc)();
}

void SceneSelectNum::Draw() const
{
	(this->*_drawFunc)();
}

void SceneSelectNum::NormalUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// UIの更新処理
	if (!_warningFrag) {
		_pUi->Update();

	}

	// Bボタンでモードセレクトに戻る
	if (Input::GetInstance().IsTrigger(INPUT_B, INPUT_PAD_1)) {

		SoundManager::GetInstance().RingSE(SE_CHARA_CANCEL);

		// タイトルに戻るフラグを立てる
		_selectFrag = true;

		// スライド画像の初期位置を設定する
		_slidePos.x = GetConstantInt("SLIDE_OUT_END_X");

		_updateFunc = &SceneSelectNum::SlideOutUpdate;
		_drawFunc = &SceneSelectNum::SlideOutDraw;
	}

	// Aボタンで選択しているボタンの処理をする
	if (Input::GetInstance().IsTrigger(INPUT_A, INPUT_PAD_1)) {

		if (_warningFrag) {
			_warningFrag = false;
			SoundManager::GetInstance().RingSE(SE_CLOSE_MENU);
		}
		else if (_pUi->GetSelect() + 1 > Input::GetInstance().GetPadNum()) {
			_warningFrag = true;
			SoundManager::GetInstance().RingSE(SE_BEEP);
		}
		else {


			// 決定音を鳴らす
			SoundManager::GetInstance().RingSE(SE_CHARA_SELECT);

			// スライド画像の初期位置を設定する
			_slidePos.x = GetConstantInt("SLIDE_IN_START_X");

			// スライドアウト処理に移行する
			_updateFunc = &SceneSelectNum::SlideOutUpdate;
			_drawFunc = &SceneSelectNum::SlideOutDraw;
		}
	}
}

void SceneSelectNum::NormalDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// UIの描画処理
	_pUi->Draw();

	// waring表示
	if (_warningFrag) {
		DrawRotaGraph(Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2, Application::GetInstance().GetConstantInt("SCREEN_HEIGHT") / 2, 1.0, 0.0, _warningHandle, true);
	}
}

void SceneSelectNum::SlideInUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// UIの更新処理
	_pUi->Update();

	// スライド画像の移動

	if (_selectFrag) {

		// スライド画像の移動
		_slidePos.x += GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったら処理の切り替え
		if (_slidePos.x >= GetConstantInt("SLIDE_IN_START_X")) {
			_updateFunc = &SceneSelectNum::NormalUpdate;
			_drawFunc = &SceneSelectNum::NormalDraw;
		}
	}
	else {

		// スライド画像の移動
		_slidePos.x -= GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったら処理の切り替え
		if (_slidePos.x <= GetConstantInt("SLIDE_OUT_END_X")) {
			_updateFunc = &SceneSelectNum::NormalUpdate;
			_drawFunc = &SceneSelectNum::NormalDraw;
		}
	}

}

void SceneSelectNum::SlideOutUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// UIの更新処理
	//_pUi->Update();

	// タイトルフラグで処理を変える
	if (_selectFrag) {
		// スライド画像の移動 
		_slidePos.x += GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったらシーン遷移
		if (_slidePos.x >= GetConstantInt("SLIDE_OUT_START_X")) {

			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelectMode>(true));

		}
	}
	else {
		// スライド画像の移動 
		_slidePos.x -= GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったらシーン遷移
		if (_slidePos.x <= GetConstantInt("SLIDE_OUT_START_X")) {

			// プレイヤーデータ
			PlayerData data;

			data.aiFlag = false;
			data.playerNum = _pUi->GetSelect();

			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelect>(_pUi->GetSelect()));
		}
	}
}

void SceneSelectNum::SlideInDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// UIの描画処理
	_pUi->Draw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}

void SceneSelectNum::SlideOutDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// UIの描画処理
	_pUi->Draw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}
