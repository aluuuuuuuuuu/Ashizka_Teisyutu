#include "ScenePve.h"
#include "StageManager.h"
#include "Player.h"
#include "StageCollisionManager.h"
#include "BulletManager.h"
#include "Input.h"
#include "MapBulletCollisionManager.h"
#include "Application.h"
#include "Vec3.h"
#include "SkyDome.h"
#include "SceneMenu.h"
#include "SceneManager.h"
#include "WedgewormManager.h"
#include "GameFlowManager.h"
#include "SceneResult.h"
#include "NumUtility.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "PlayerBulletCollisionManager.h"
#include "ItemManager.h"

ScenePve::ScenePve(PlayerData data) :
	_frame(Application::GetInstance().GetConstantInt("FRAME_NUM") * 2)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/ScenePve.csv");

	// タイトルのBGMを止める
	SoundManager::GetInstance().StopBGM(BGM_OPENING);

	// 各クラスのインスタンス作成
	{
		_pWedgewormManager = std::make_shared<WedgewormManager>();	// 禊虫マネージャー
		_pBulletManager = std::make_shared <BulletManager>(_pBulletCollsionManager, _pWedgewormManager);	// バレットマネージャー
		_pStage = std::make_shared<StageManager>();													// ステージマネージャー
		_pBulletCollsionManager = std::make_shared<MapBulletCollisionManager>(_pStage);				// バレットコリジョンマネージャー
		_pStageCollisionManager = std::make_shared<StageCollisionManager>(_pStage);					// ステージコリジョンマネージャー
		_pPlayerManager = std::make_shared<PlayerManager>(_pStage, _pBulletManager, data);	// プレイヤーマネージャー
		_pSkyDome = std::make_shared<SkyDome>();	// スカイドーム
		_pGameFlowManager = std::make_shared<GameFlowManager>(_pPlayerManager);	// ゲームフローマネージャー
		_pNum = std::make_shared<NumUtility>(GetConstantFloat("NUM_SIZE"), Vec2{ GetConstantFloat("NUM_POS_X"),GetConstantFloat("NUM_POS_Y") });	// 数字ユーティリティ
		_pPlayerBulletCollisionManager = std::make_shared<PlayerBulletCollisionManager>(_pBulletManager, _pPlayerManager);
		_pItemManager = std::make_shared<ItemManager>(_pPlayerManager, _pStage);
	}

	// AIの追加
	_pPlayerManager->AddAi();

	// ライトの設定
	{
		auto& app = Application::GetInstance();
		// ライティングを使用する
		SetUseLighting(true);

		// ライトのカラーを調整する
		SetLightDifColor(GetColorF(app.GetConstantFloat("LIGHT_COLOR_R"),
			app.GetConstantFloat("LIGHT_COLOR_G"),
			app.GetConstantFloat("LIGHT_COLOR_B"),
			app.GetConstantFloat("LIGHT_COLOR_ALPHA")));

		// ライトの角度を設定
		SetLightDirection(VECTOR{ app.GetConstantFloat("LIGHT_DIRECTION_X"),
			app.GetConstantFloat("LIGHT_DIRECTION_Y"),
			app.GetConstantFloat("LIGHT_DIRECTION_Z"), });
	}

	// 関数ポインタの初期化
	{
		_updateFunc = &ScenePve::FadeInUpdate;
		_drawFunc = &ScenePve::FadeInDraw;
	}

	// プレイヤーの更新処理
	_pPlayerManager->Update();
}

ScenePve::~ScenePve()
{
	// ライトの削除
	DeleteLightHandleAll();

	// 描画範囲と描画先の中心の初期化
	SetDrawArea(0, 0, Application::GetInstance().GetConstantInt("SCREEN_WIDTH"), Application::GetInstance().GetConstantInt("SCREEN_HEIGHT"));
	SetCameraScreenCenter(static_cast<float>(Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2), static_cast<float>(Application::GetInstance().GetConstantInt("SCREEN_HEIGHT") / 2));
}

void ScenePve::Update()
{
	(this->*_updateFunc)();
}

void ScenePve::Draw() const
{
	(this->*_drawFunc)();
}

void ScenePve::NormalUpdate()
{
	// BGMを再生	
	SoundManager::GetInstance().StartBGM(BGM_BATTLE);

	// メニュー画面を開く
	if (Input::GetInstance().IsTrigger(INPUT_START, INPUT_PAD_1)) {
		SceneManager::GetInstance().PushScene(std::make_shared<SceneMenu>(INPUT_PAD_1));
	}

	// スカイドームの更新処理
	_pSkyDome->Update();

	// プレイヤーの更新処理
	_pPlayerManager->Update();

	// アイテムの更新処理
	_pItemManager->Update();

	// ゲームフローの更新処理
	_pGameFlowManager->Update();

	// バレットの更新
	_pBulletManager->Update();

	// 禊虫の更新
	_pWedgewormManager->Update();

	// プレイヤーコリジョンマネージャーの更新
	_pPlayerBulletCollisionManager->Update();

	// ゲームが終了していたら終了時の処理に移る
	if (_pGameFlowManager->GetGameEnd()) {
		_updateFunc = &ScenePve::EndUpdate;
		_drawFunc = &ScenePve::EndDraw;
	}

	// 時間の更新処理
	_pNum->Update(_pGameFlowManager->GetGameTime());
}

void ScenePve::NormalDraw() const
{
	// カメラの設定
	_pPlayerManager->CameraSet(0);

	//スカイドームの描画
	_pSkyDome->Draw();

	// バレットの描画
	_pBulletManager->Draw();

	// ステージの描画
	_pStage->DrawStage();

	// 禊虫の描画
	_pWedgewormManager->Draw();

	// アイテムの描画
	_pItemManager->Draw();

	// エフェクトの描画
	EffectManager::GetInstance().Draw();

	// プレイヤーの描画
	_pPlayerManager->Draw(0);

	// 時間の更新処理
	_pNum->Draw();

}

void ScenePve::EndUpdate()
{
	// ゲームフローマネージャーの更新
	_pGameFlowManager->Update();

	_pPlayerManager->Update();

	_frame++;

	// ゲームが終了してから2秒たてばリザルト画面へ移行
	if (_frame >= Application::GetInstance().GetConstantInt("FRAME_NUM") * 2) {

		SoundManager::GetInstance().StopBGM(BGM_BATTLE);
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneResult>(_pPlayerManager->GetPlayerData(), _pGameFlowManager->GetGameTime()));
	}
}

void ScenePve::EndDraw() const
{
	// 通常の描画
	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, 1980, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void ScenePve::FadeInUpdate()
{
	// 通常の更新
	NormalUpdate();

	_frame--;
	if (_frame == 0) {
		_updateFunc = &ScenePve::NormalUpdate;
		_drawFunc = &ScenePve::NormalDraw;
	}
}

void ScenePve::FadeInDraw() const
{
	// 通常の描画
	NormalDraw();

	//フェード暗幕
	int alpha = static_cast<int>(255 * ((float)_frame / Application::GetInstance().GetConstantInt("FRAME_NUM") * 2));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, 1980, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
