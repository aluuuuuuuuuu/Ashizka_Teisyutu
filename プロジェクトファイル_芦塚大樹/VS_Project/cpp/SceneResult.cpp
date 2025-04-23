#include "SceneResult.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneMenu.h"
#include "SceneTitle.h"
#include "ScenePvp.h"
#include "SceneManager.h"
#include "ResultUi.h"
#include "NumUtility.h"
#include "ResultCharacter.h"
#include "SoundManager.h"
#include "ScenePve.h"
#include "SkyDome.h"
#include "Effekseer.h"
#include <EffekseerForDXLib.h>
#include "Application.h"

SceneResult::SceneResult(PlayerData data, int gameTime) :
	_playerData(data),
	_frame(0),
	_nextScene(0)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/SceneResult.csv");

	// キャラクターの作成
	_pCharacter = std::make_shared<ResultCharacter>(data.winner);

	// 数字の作成
	_pNum = std::make_shared<NumUtility>(GetConstantFloat("NUM_SIZE"), Vec2{ GetConstantFloat("NUM_POS_X"),GetConstantFloat("NUM_POS_Y") }, gameTime);

	// スカイドームの作成
	_pSkyDome = std::make_shared<SkyDome>();

	// リザルトUIの作成
	_pResultUi = std::make_shared<ResultUi>(gameTime);

	// 関数ポインタの初期化
	_updateFunc = &SceneResult::NormalUpdate;
	_drawFunc = &SceneResult::NormalDraw;

	// キャラクターの更新処理
	_pCharacter->Update();

	// シングルの再生
	SoundManager::GetInstance().RingSE(SE_RESULT);
}

SceneResult::~SceneResult()
{
}

void SceneResult::Update()
{
	(this->*_updateFunc)();


	// えふぇくしあの更新
	UpdateEffekseer3D();
}

void SceneResult::Draw() const
{
	(this->*_drawFunc)();
}

void SceneResult::NormalUpdate()
{
	auto plNum = _playerData.playerNum;

	// AIフラグが立っていたらプレイヤーの人数を一人減らす
	if (_playerData.aiFlag) {
		plNum--;
	}

	// プレイヤーの数だけ判定を行う
	for (int num = 0; num <= plNum; num++) {

		// Aボタンが押されたらリスタート
		if (Input::GetInstance().IsTrigger(INPUT_A, num)) {

			// AI戦であればPVEシーンに戻る
			if (_playerData.aiFlag) {
				_nextScene = SCENE_PVE;
			}
			else {
				_nextScene = SCENE_PVP;
			}

			// フェードアウトに移行
			_updateFunc = &SceneResult::FadeOutUpdate;
			_drawFunc = &SceneResult::FadeOutDraw;
		}

		// Bボタンが押されたらタイトル画面
		if (Input::GetInstance().IsTrigger(INPUT_B, num)) {
			_nextScene = SCENE_TITLE;
			_updateFunc = &SceneResult::FadeOutUpdate;
			_drawFunc = &SceneResult::FadeOutDraw;
		}

		// STARTボタンが押されたらメニューを開く
		if (Input::GetInstance().IsTrigger(INPUT_START, num)) {
			SceneManager::GetInstance().PushScene(std::make_shared<SceneMenu>(num));
		}
	}

	// スカイドームの更新
	_pSkyDome->Update();

	// キャラクターの更新処理
	_pCharacter->Update();
}

void SceneResult::NormalDraw() const
{
	// スカイドームの描画
	_pSkyDome->Draw();

	// キャラクターの描画
	_pCharacter->Draw();

	// リザルトUIの描画
	_pResultUi->Draw();

	// 数字の描画
	_pNum->Draw();
}

void SceneResult::FadeOutUpdate()
{
	_frame++;

	// フェードが終了したら
	if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM") * 2) {
		// 次のシーンに切り替える
		switch (_nextScene)
		{
		case SCENE_PVP:
			SceneManager::GetInstance().ChangeScene(std::make_shared<ScenePvp>(_playerData));
			break;
		case SCENE_PVE:
			_playerData.playerNum--;
			SceneManager::GetInstance().ChangeScene(std::make_shared<ScenePve>(_playerData));
			break;
		case SCENE_TITLE:
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>(false));
			break;
		default:
			break;
		}
	}
}

void SceneResult::FadeOutDraw() const
{
	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Application::GetInstance().GetConstantInt("SCREEN_WIDTH"), Application::GetInstance().GetConstantInt("SCREEN_HEIGHT"), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
