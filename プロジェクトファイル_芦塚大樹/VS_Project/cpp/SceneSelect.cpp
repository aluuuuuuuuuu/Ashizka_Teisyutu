#include "SceneSelect.h"
#include "Input.h"
#include "SceneManager.h"
#include "SelectFinger.h"
#include "CharacterCard.h"
#include "CharacterSelectManager.h"
#include "SelectUI.h"
#include "SceneTitle.h"
#include "SoundManager.h"
#include "SkyDome.h"
#include "ScenePause.h"
#include "SceneSelectMode.h"
#include "Application.h"

SceneSelect::SceneSelect(int num) :
	_frame(0)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/SceneSelect.csv");

	// 関数ポインタの初期化
	_updateFunc = &SceneSelect::SlideInUpdate;
	_drawFunc = &SceneSelect::SlideInDraw;

	// プレイヤーデータの初期化
	for (auto& pl : _plData.character) {
		pl = -1;
	}
	_plData.playerNum = num;

	// インスタンスの作成
	{
		_pSelectManager = std::make_shared<CharacterSelectManager>(_plData);	// セレクトマネージャー
		_pUi = std::make_shared<SelectUI>();	// ui
		_pSkyDome = std::make_shared<SkyDome>();	// スカイドーム
	}

	// スライド画像のロード
	_slideHandle = LoadGraph("data/image/Slide.png");

	_slidePos = Vec2{ GetConstantInt("SLIDE_OUT_START_X"),GetConstantInt("SLIDE_OUT_START_Y") };
}

SceneSelect::~SceneSelect()
{
	DeleteGraph(_slideHandle);
}

void SceneSelect::Update()
{
	(this->*_updateFunc)();
}

void SceneSelect::Draw() const
{
	(this->*_drawFunc)();
}

void SceneSelect::CharacterSelectUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// セレクトマネージャーの更新
	_pSelectManager->Update();

	// スタートボタンが押されたらゲームシーンに移行する
	if (_pSelectManager->GetStart()) {

		// プレイヤーデータを作成する
		_pSelectManager->CreateData();

		// フェードアウトに移行
		_updateFunc = &SceneSelect::FadeOutUpdate;
		_drawFunc = &SceneSelect::FadeOutDraw;
	}

	// マネージャーの戻るフラグが立っていたら人数選択に戻る
	if (_pSelectManager->GetReturnFlag()) {

		// キャンセル音を鳴らす
		SoundManager::GetInstance().RingSE(SE_CHARA_CANCEL);

		// フェードアウトに移行
		_updateFunc = &SceneSelect::SlideOutUpdate;
		_drawFunc = &SceneSelect::SlideOutDraw;
	}
}

void SceneSelect::CharacterSelectDraw() const
{
	// スカイドームの描画
	_pSkyDome->Draw();

	// uiの描画
	_pUi->Draw();

	// セレクトマネージャーの描画処理
	_pSelectManager->Draw();
}

void SceneSelect::SlideInUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// スライド画像の移動
	_slidePos.x -= GetConstantFloat("SLIDE_MOVE_SCALE");
	if (_slidePos.x <= GetConstantInt("SLIDE_IN_END_X")) {
		_updateFunc = &SceneSelect::CharacterSelectUpdate;
		_drawFunc = &SceneSelect::CharacterSelectDraw;
	}
}

void SceneSelect::SlideInDraw() const
{
	// 通常の描画も行う
	CharacterSelectDraw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}

void SceneSelect::SlideOutUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	_slidePos.x += GetConstantFloat("SLIDE_MOVE_SCALE");
	if (_slidePos.x >= GetConstantInt("SLIDE_OUT_START_X")) {
		// 次のシーンに移行する
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelectMode>(true));
	}
}

void SceneSelect::SlideOutDraw() const
{
	// 通常の描画も行う
	CharacterSelectDraw();

	// スライド画像の描画
	DrawGraph(_slidePos.intX(), _slidePos.intY(), _slideHandle, true);
}

void SceneSelect::FadeOutUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	_frame++;
	if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM")) {

		// 次のシーンに移行する
		SceneManager::GetInstance().ChangeScene(std::make_shared<ScenePause>(_plData));
	}
}

void SceneSelect::FadeOutDraw() const
{
	// 通常の描画も行う
	CharacterSelectDraw();

	//フェード暗幕
	int alpha = (int)(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM")));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Application::GetInstance().GetConstantInt("SCREEN_WIDTH"), Application::GetInstance().GetConstantInt("SCREEN_HEIGHT"), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
