#include "SceneSelectMode.h"
#include "SelectModeUi.h"
#include "DxLib.h"
#include "SkyDome.h"
#include "Input.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "SceneMenu.h"
#include "SceneSelect.h"
#include "SceneTutorial.h"
#include "SceneSelectNum.h"
#include "Application.h"

SceneSelectMode::SceneSelectMode(bool slideInFlag) :
	_titleFrag(false),
	_returnSelectFrag(slideInFlag),
	_frame(0)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/SceneSelectMode.csv");

	// 関数ポインタの初期化
	_updateFunc = &SceneSelectMode::SlideInUpdate;
	_drawFunc = &SceneSelectMode::SlideInDraw;

	// UIインスタンス作成
	_pUi = std::make_shared<SelectModeUi>(true);

	// スカイドームインスタンス作成
	_pSkyDome = std::make_shared<SkyDome>();

	// スライド画像のロード
	_slideHandle = LoadGraph("data/image/Slide.png");

	// セレクトシーンから戻っていたらスライド画像は左から開く
	if (_returnSelectFrag) {
		_slidePos.x = GetConstantInt("SLIDE_OUT_START_X");
	}
	else {
		_slidePos.x = GetConstantInt("SLIDE_OUT_START_X");
	}

	// カメラの初期化
	SetCameraPositionAndTarget_UpVecY(VECTOR{ GetConstantFloat("CAMERA_POS_X"),GetConstantFloat("CAMERA_POS_Y"),GetConstantFloat("CAMERA_POS_Z") },
		VECTOR{ GetConstantFloat("CAMERA_TARGET_X"), GetConstantFloat("CAMERA_TARGET_Y"), GetConstantFloat("CAMERA_TARGET_Z") });

	SetCameraNearFar(GetConstantFloat("CAMERA_NEAR"), GetConstantFloat("CAMERA_FAR"));

	// BGMの再生
	SoundManager::GetInstance().StartBGM(BGM_OPENING);
}

SceneSelectMode::~SceneSelectMode()
{
	DeleteGraph(_slideHandle);
}

void SceneSelectMode::Update()
{
	(this->*_updateFunc)();
}

void SceneSelectMode::Draw() const
{
	(this->*_drawFunc)();
}

void SceneSelectMode::NormalUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// UIの更新処理
	_pUi->Update();

	// Bボタンでタイトルに戻る
	if (Input::GetInstance().IsTrigger(INPUT_B, INPUT_PAD_1)) {

		SoundManager::GetInstance().RingSE(SE_CHARA_CANCEL);

		// タイトルに戻るフラグを立てる
		_titleFrag = true;

		// スライド画像の初期位置を設定する
		_slidePos.x = GetConstantInt("SLIDE_OUT_END_X");

		_updateFunc = &SceneSelectMode::SlideOutUpdate;
		_drawFunc = &SceneSelectMode::SlideOutDraw;
	}

	// Aボタンで選択しているボタンの処理をする
	if (Input::GetInstance().IsTrigger(INPUT_A, INPUT_PAD_1)) {

		// 決定音を鳴らす
		SoundManager::GetInstance().RingSE(SE_CHARA_SELECT);

		switch (_pUi->GetSelect())
		{
		case SOLO_MODE:
		case MULTI_MODE:
		case TUTORIAL_MODE:

			// スライド画像の初期位置を設定する
			_slidePos.x = GetConstantInt("SLIDE_IN_START_X");

			// スライドアウト処理に移行する
			_updateFunc = &SceneSelectMode::SlideOutUpdate;
			_drawFunc = &SceneSelectMode::SlideOutDraw;
			break;
		case OPTION_MODE:
			// メニューシーンを追加する
			SceneManager::GetInstance().PushScene(std::make_shared<SceneMenu>(INPUT_PAD_1));
			break;
		default:
			break;
		}
	}
}

void SceneSelectMode::NormalDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// UIの描画処理
	_pUi->Draw();
}

void SceneSelectMode::SlideInUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// UIの更新処理
	_pUi->Update();

	// スライド画像の移動

	if (_returnSelectFrag) {

		// スライド画像の移動
		_slidePos.x += GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったら処理の切り替え
		if (_slidePos.x >= GetConstantInt("SLIDE_IN_START_X")) {
			_updateFunc = &SceneSelectMode::NormalUpdate;
			_drawFunc = &SceneSelectMode::NormalDraw;
		}
	}
	else {

		// スライド画像の移動
		_slidePos.x -= GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったら処理の切り替え
		if (_slidePos.x <= GetConstantInt("SLIDE_OUT_END_X")) {
			_updateFunc = &SceneSelectMode::NormalUpdate;
			_drawFunc = &SceneSelectMode::NormalDraw;
		}
	}

}

void SceneSelectMode::SlideOutUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// UIの更新処理
	//_pUi->Update();

	// タイトルフラグで処理を変える
	if (_titleFrag) {
		// スライド画像の移動 
		_slidePos.x += GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったらシーン遷移
		if (_slidePos.x >= GetConstantInt("SLIDE_OUT_START_X")) {
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>(true));
		}
	}
	else {
		// スライド画像の移動 
		_slidePos.x -= GetConstantInt("SLIDE_MOVE_SCALE");

		// 移動が終わったらシーン遷移
		if (_slidePos.x <= GetConstantInt("SLIDE_OUT_START_X")) {
			switch (_pUi->GetSelect())
			{
			case SOLO_MODE:
				SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelect>(0));
				break;
			case MULTI_MODE:

				SceneManager::GetInstance().ChangeScene(std::make_shared <SceneSelectNum>());
				break;
			case TUTORIAL_MODE:
				SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTutorial>());
				break;
			case OPTION_MODE:
			default:
				break;
			}
		}
	}
}

void SceneSelectMode::SlideInDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// UIの描画処理
	_pUi->Draw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}

void SceneSelectMode::SlideOutDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// UIの描画処理
	_pUi->Draw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}
