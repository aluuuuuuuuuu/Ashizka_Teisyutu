#include "SceneTitle.h"
#include "SceneManager.h"
#include "Input.h"
#include "SoundManager.h"
#include "Crown.h"
#include "Logo.h"
#include "AnyPushButton.h"
#include "FallCharactor.h"
#include "SkyDome.h"
#include "TitlePlayer.h"
#include "SceneSelectMode.h"
#include "SceneCredit.h"
#include "Application.h"

SceneTitle::SceneTitle(bool slidInFlag) :
	_frame(Application::GetInstance().GetConstantInt("FRAME_NUM")),
	_selectDrawFlag(false),
	_endFrag(false)
{
	// 乱数生成器の初期化
	srand(static_cast<unsigned int>(time(nullptr)));

	// 定数ファイルの読み込み
	ReadCSV("data/constant/SceneTitle.csv");

	// カメラの初期化
	SetCameraPositionAndTarget_UpVecY(VECTOR{ GetConstantFloat("CAMERA_POS_X"),GetConstantFloat("CAMERA_POS_Y"),GetConstantFloat("CAMERA_POS_Z") },
		VECTOR{ GetConstantFloat("CAMERA_TARGET_X"), GetConstantFloat("CAMERA_TARGET_Y"), GetConstantFloat("CAMERA_TARGET_Z") });

	SetCameraNearFar(GetConstantFloat("CAMERA_NEAR"), GetConstantFloat("CAMERA_FAR"));

	// 関数ポインタの初期化
	{
		if (slidInFlag) {
			_slidePos.x = GetConstantInt("SLIDE_MOVE_SCALE");
			_updateFunc = &SceneTitle::SlideInUpdate;
			_drawFunc = &SceneTitle::SlideInDraw;
		}
		else {
			_slidePos.x = GetConstantInt("SLIDE_IN_START_X");
			_updateFunc = &SceneTitle::FadeInUpdate;
			_drawFunc = &SceneTitle::FadeInDraw;
		}
	}

	// 各インスタンスの作成
	{
		_pCrown = std::make_shared<Crown>(*this);	// 王冠
		_pLogo = std::make_shared <Logo>(*this);	// ロゴ
		_pText = std::make_shared<AnyPushButton>(*this);	// 文章
		_pFallCharactor = std::make_shared<FallCharactor>(); // 落下キャラクター
		_pSkyDome = std::make_shared<SkyDome>(); // スカイドーム
	}

	// 両翼のモデル
	switch (rand() % 4)
	{
	case 0:
		_pModel1 = std::make_shared<TitlePlayer>(0, 0);
		_pModel2 = std::make_shared<TitlePlayer>(1, 1);
		break;
	case 1:
		_pModel1 = std::make_shared<TitlePlayer>(0, 2);
		_pModel2 = std::make_shared<TitlePlayer>(1, 3);
		break;
	case 2:
		_pModel1 = std::make_shared<TitlePlayer>(0, 0);
		_pModel2 = std::make_shared<TitlePlayer>(1, 3);
		break;
	case 3:
		_pModel1 = std::make_shared<TitlePlayer>(0, 1);
		_pModel2 = std::make_shared<TitlePlayer>(1, 2);
		break;
	default:
		break;
	}

	// スライド画像のロード
	_slideHandle = LoadGraph("data/image/Slide.png");

	// セリフ画像のロード
	_serihu = LoadGraph("data/image/message2.png");

	_serihu2 = LoadGraph("data/image/message3.png");

	_serihu3 = LoadGraph("data/image/message4.png");

	// ゲーム終了画像のロード
	_endHandle = LoadGraph("data/image/GameEnda.png");

	// ゲーム終了ボタンテキスト画像
	_gameEndHandle = LoadGraph("data/image/startToGameEnd.png");

	// オープニングのテーマを再生する
	SoundManager::GetInstance().StopBGM(BGM_THEME);
	SoundManager::GetInstance().StartBGM(BGM_OPENING);

	// ライトの設定
	SetLightDirection(VECTOR{ 150.0f, 0.0f, 0.0f });
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(_serihu);
	DeleteGraph(_serihu2);
	DeleteGraph(_serihu3);
	DeleteGraph(_slideHandle);
	DeleteGraph(_endHandle);
	DeleteGraph(_gameEndHandle);
}

void SceneTitle::Update()
{
	(this->*_updateFunc)();
}

void SceneTitle::Draw() const
{
	(this->*_drawFunc)();
}

void SceneTitle::NormalUpdate()
{
	// 落下キャラクターの更新処理
	_pFallCharactor->Update();

	// 王冠の更新処理
	_pCrown->Update();

	// 文章の更新処理
	_pText->Update();

	// スカイドームの更新処理
	_pSkyDome->Update();

	// 両翼の更新処理
	_pModel1->Update();
	_pModel2->Update();

	// Xボタンが押されたらクレジット表示
	if (Input::GetInstance().GetPadNum() != 0) {
		for (int num = 0; num < Input::GetInstance().GetPadNum(); num++) {
			if (Input::GetInstance().IsTrigger(INPUT_X, num) && !_endFrag) {

				_frame = 0;

				// フェードアウト
				_updateFunc = &SceneTitle::NormalFadeOutUpdate;
				_drawFunc = &SceneTitle::NormalFadeDraw;
			}
			else if (Input::GetInstance().IsTrigger(INPUT_A, num) && !_endFrag) {

				// 決定音を鳴らす
				SoundManager::GetInstance().RingSE(SE_TITLE_START);

				// 人数選択へ移行
				_updateFunc = &SceneTitle::SlideOutUpdate;
				_drawFunc = &SceneTitle::SlideOutDraw;
			}
			else if (Input::GetInstance().IsTrigger(INPUT_A, num) && _endFrag) {
				SceneManager::GetInstance().GameEnd();
			}
			else if (Input::GetInstance().IsTrigger(INPUT_B, num) && _endFrag) {
				_endFrag = false;
			}

			// startボタンが押されたらゲーム終了確認画像を表示
			if (Input::GetInstance().IsTrigger(INPUT_START, num)) {
				if (_endFrag) {
					_endFrag = false;
				}
				else {
					_endFrag = true;
				}
			}
		}
	}


}

void SceneTitle::NormalDraw() const
{
	// スカイドームの描画
	_pSkyDome->Draw();

	// 落下キャラクターの描画
	_pFallCharactor->Draw();

	// ロゴの描画
	_pLogo->Draw();

	// 王冠画像の描画
	_pCrown->Draw();

	// 文章画像の描画
	_pText->Draw();

	// メッセージの描画処理
	if (Input::GetInstance().GetPadNum() == 0) {
		DrawGraph(GetConstantInt("MASSAGE_POS_X"), GetConstantInt("MASSAGE_POS_Y"), _serihu, true);
	}
	else {
		DrawGraph(GetConstantInt("MASSAGE_POS_X"), GetConstantInt("MASSAGE_POS_Y"), _serihu2, true);
	}

	// 両翼の描画
	_pModel1->Draw();
	_pModel2->Draw();

	// ゲーム終了ガイド画像を表示
	DrawGraph(GetConstantInt("GAME_END_POS_X"), GetConstantInt("GAME_END_POS_Y"), _gameEndHandle, true);

	auto& app = Application::GetInstance();
	// ゲーム終了画像の描画
	if (_endFrag) DrawRotaGraph(app.GetConstantInt("SCREEN_WIDTH") / 2, app.GetConstantInt("SCREEN_HEIGHT") / 2, 1.0, 0.0, _endHandle, false);
}

void SceneTitle::SlideInUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// スライド画像の移動
	_slidePos.x += GetConstantInt("SLIDE_MOVE_SCALE");

	// 両翼の更新処理
	_pModel1->Update();
	_pModel2->Update();

	// 移動が終わったら通常の状態に遷移
	if (_slidePos.x > GetConstantInt("SLIDE_IN_START_X")) {
		_updateFunc = &SceneTitle::NormalUpdate;
		_drawFunc = &SceneTitle::NormalDraw;
	}
}

void SceneTitle::SlideOutUpdate()
{
	// スライド画像の移動 
	_slidePos.x -= GetConstantInt("SLIDE_MOVE_SCALE");

	// スカイドームの更新処理
	_pSkyDome->Update();

	_pText->Update();

	// 両翼の更新処理
	_pModel1->Update();
	_pModel2->Update();

	// 移動が終わったらシーン遷移
	if (_slidePos.x <= GetConstantInt("SLIDE_OUT_START_X")) {
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelectMode>(false));
	}
}

void SceneTitle::SlideInDraw() const
{
	// 通常の描画も行う
	NormalDraw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}

void SceneTitle::SlideOutDraw() const
{
	// スカイドームの描画処理
	_pSkyDome->Draw();

	// 人数選択の描画
	NormalDraw();

	// 両翼の描画
	_pModel1->Draw();
	_pModel2->Draw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}

void SceneTitle::FadeInUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// 王冠の更新処理
	_pCrown->Update();

	// 文章の更新処理
	_pText->Update();

	// 両翼の更新処理
	_pModel1->Update();
	_pModel2->Update();

	_frame--;
	if (_frame == 0) {
		_updateFunc = &SceneTitle::NormalUpdate;
		_drawFunc = &SceneTitle::NormalDraw;
	}
}

void SceneTitle::FadeInDraw() const
{
	NormalDraw();
	auto& app = Application::GetInstance();
	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, app.GetConstantInt("SCREEN_WIDTH"), app.GetConstantInt("SCREEN_HEIGHT"), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::NormalFadeInUpdate()
{
	// 落下キャラクターの更新処理
	_pFallCharactor->Update();

	// 王冠の更新処理
	_pCrown->Update();

	// 文章の更新処理
	_pText->Update();

	// スカイドームの更新処理
	_pSkyDome->Update();

	// 両翼の更新処理
	_pModel1->Update();
	_pModel2->Update();

	_frame--;
	if (_frame == 0) {

		_updateFunc = &SceneTitle::NormalUpdate;
		_drawFunc = &SceneTitle::NormalDraw;
	}
}

void SceneTitle::NormalFadeOutUpdate()
{
	// 落下キャラクターの更新処理
	_pFallCharactor->Update();

	// 王冠の更新処理
	_pCrown->Update();

	// 文章の更新処理
	_pText->Update();

	// スカイドームの更新処理
	_pSkyDome->Update();

	// 両翼の更新処理
	_pModel1->Update();
	_pModel2->Update();

	_frame++;
	if (_frame == Application::GetInstance().GetConstantInt("FRAME_NUM")) {

		// シーンの切り替え
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneCredit>());
	}
}

void SceneTitle::NormalFadeDraw() const
{
	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM")));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 1980, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}