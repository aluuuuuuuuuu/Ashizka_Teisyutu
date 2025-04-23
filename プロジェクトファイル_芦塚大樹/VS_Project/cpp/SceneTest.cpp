#include "SceneTest.h"
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

SceneTest::SceneTest(PlayerData& data)
{
	// タイトルのBGMを止める
	SoundManager::GetInstance().StopBGM(BGM_OPENING);

	// 各クラスのインスタンス作成
	{
		_pWedgewormManager = std::make_shared<WedgewormManager>();	// 禊虫マネージャー
		_pBulletManager = std::make_shared <BulletManager>(_pBulletCollsionManager,_pWedgewormManager);				// バレットマネージャー
		_pStage = std::make_shared<StageManager>();													// ステージマネージャー
		_pBulletCollsionManager = std::make_shared<MapBulletCollisionManager>(_pStage);				// バレットコリジョンマネージャー
		_pStageCollisionManager = std::make_shared<StageCollisionManager>(_pStage);					// ステージコリジョンマネージャー
		_pPlayerManager = std::make_shared<PlayerManager>(_pStage, _pBulletManager, data);	// プレイヤーマネージャー
		_pSkyDome = std::make_shared<SkyDome>();	// スカイドーム
		_pGameFlowManager = std::make_shared<GameFlowManager>(_pPlayerManager);	// ゲームフローマネージャー
		_pNum = std::make_shared<NumUtility>(0.5f,Vec2{ 734,100 });	// 数字ユーティリティ
	}

	// ライトの設定
	{
		// ライティングを使用する
		SetUseLighting(true);

		// ライトのカラーを調整する
		SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));

		// ライトの角度を設定
		SetLightDirection(VECTOR{ 1.0f, -1.0f ,0.0f, });
	}

	// 関数ポインタの初期化
	{
		_updateFunc = &SceneTest::NormalUpdate;
		_drawFunc = &SceneTest::NormalDraw;
	}
}

SceneTest::~SceneTest()
{
	// ライトの削除
	DeleteLightHandleAll();

	// 描画範囲と描画先の中心の初期化
	SetDrawArea(0, 0, Application::GetInstance().GetConstantInt("SCREEN_WIDTH"), Application::GetInstance().GetConstantInt("SCREEN_HEIGHT"));
	SetCameraScreenCenter(static_cast<float>(Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2), static_cast<float>(Application::GetInstance().GetConstantInt("SCREEN_HEIGHT") / 2));
}

void SceneTest::Update()
{
	(this->*_updateFunc)();
}

void SceneTest::Draw() const
{
	(this->*_drawFunc)();
}

void SceneTest::NormalUpdate()
{
	SoundManager::GetInstance().StartBGM(BGM_BATTLE);

	// メニュー画面を開く
	for (int num = 0; num < _pPlayerManager->GetPlayerNum(); num++) {
		if (Input::GetInstance().IsTrigger(INPUT_START, num)) {
			SceneManager::GetInstance().PushScene(std::make_shared<SceneMenu>(num));
		}
	}

	// スカイドームの更新処理
	_pSkyDome->Update();

	// プレイヤーの更新処理
	_pPlayerManager->Update();

	// ゲームフローの更新処理
	_pGameFlowManager->Update();

	// バレットの更新
	_pBulletManager->Update();

	// 禊虫の更新
	_pWedgewormManager->Update();

	// ゲームが終了していたら終了時の処理に移る
	if (_pGameFlowManager->GetGameEnd()) {
		_updateFunc = &SceneTest::EndUpdate;
		_drawFunc = &SceneTest::EndDraw;
	}

	// 時間の更新処理
	_pNum->Update(_pGameFlowManager->GetGameTime());
}

void SceneTest::NormalDraw() const
{
	// プレイヤーの画面の数だけ描画する
	for (int i = 0; i < _pPlayerManager->GetPlayerNum(); i++) {

		// カメラの設定
		_pPlayerManager->CameraSet(i);

		// 描画範囲の設定
		SetDrawArea(_pPlayerManager->GetArea(i).a, _pPlayerManager->GetArea(i).b, _pPlayerManager->GetArea(i).c, _pPlayerManager->GetArea(i).d);

		// 描画先の中心を設定
		SetCameraScreenCenter(static_cast<float>(_pPlayerManager->GetCenter(i).a), static_cast<float>(_pPlayerManager->GetCenter(i).b));

		//スカイドームの描画
		_pSkyDome->Draw();

		// バレットの描画
		_pBulletManager->Draw();

		// ステージの描画
		_pStage->DrawStage();

		// 禊虫の描画
		_pWedgewormManager->Draw();

		// プレイヤーの描画
		_pPlayerManager->Draw(i);
	}

	// 描画範囲の設定
	SetDrawArea(0, 0, 1920, 1080);

	// 描画先の中心を設定
	SetCameraScreenCenter(static_cast<float>(1920 / 2), static_cast<float>(1080 / 2));

	// 数字の描画
	_pNum->Draw();

}

void SceneTest::EndUpdate()
{	
	// ゲームフローマネージャーの更新
	_pGameFlowManager->Update();

	// ゲームが終了してから１２０フレームたてばリザルト画面へ移行
	if (_pGameFlowManager->GetFlameCount() >= 120) {
		SoundManager::GetInstance().StopBGM(BGM_BATTLE);
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneResult>(_pPlayerManager->GetPlayerData(),_pGameFlowManager->GetGameTime()));
	}
}

void SceneTest::EndDraw() const
{
	// プレイヤーの画面の数だけ描画する
	for (int i = 0; i < _pPlayerManager->GetPlayerNum(); i++) {

		// カメラの設定
		_pPlayerManager->CameraSet(i);

		// 描画範囲の設定
		SetDrawArea(_pPlayerManager->GetArea(i).a, _pPlayerManager->GetArea(i).b, _pPlayerManager->GetArea(i).c, _pPlayerManager->GetArea(i).d);

		// 描画先の中心を設定
		SetCameraScreenCenter(static_cast<float>(_pPlayerManager->GetCenter(i).a), static_cast<float>(_pPlayerManager->GetCenter(i).b));

		//スカイドームの描画
		_pSkyDome->Draw();

		// バレットの描画
		_pBulletManager->Draw();

		// ステージの描画
		_pStage->DrawStage();

		// 禊虫の描画
		_pWedgewormManager->Draw();

		// プレイヤーの描画
		_pPlayerManager->Draw(i);
	}
}
