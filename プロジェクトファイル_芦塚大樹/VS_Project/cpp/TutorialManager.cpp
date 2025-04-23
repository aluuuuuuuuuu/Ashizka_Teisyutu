#include "TutorialManager.h"
#include "BulletManager.h"
#include "StageManager.h"
#include "MapBulletCollisionManager.h"
#include "StageCollisionManager.h"
#include "PlayerManager.h"
#include "SkyDome.h"
#include "WedgewormManager.h"
#include "EffectManager.h"
#include "Input.h"
#include "Application.h"

TutorialManager::TutorialManager() :
	_frame(0),
	_tutorialProgress(GUIDE_CAMERA),
	_endFrag(false),
	_shotCount(0),
	_finishFrag(false)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/TutorialManager.csv");

	_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));


	PlayerData data;

	data.aiFlag = false;
	data.character[PLAYER_ONE] = 0;
	data.playerNum = PLAYER_ONE;

	// 各クラスのインスタンス作成
	{
		_pWedgewormManager = std::make_shared<WedgewormManager>();	// 禊虫マネージャ
		_pBulletManager = std::make_shared <BulletManager>(_pBulletCollsionManager, _pWedgewormManager);	// バレットマネージャー
		_pStage = std::make_shared<StageManager>();													// ステージマネージャー
		_pBulletCollsionManager = std::make_shared<MapBulletCollisionManager>(_pStage);				// バレットコリジョンマネージャー
		_pStageCollisionManager = std::make_shared<StageCollisionManager>(_pStage);					// ステージコリジョンマネージャー
		_pPlayerManager = std::make_shared<PlayerManager>(_pStage, _pBulletManager, data);	// プレイヤーマネージャー
		_pSkyDome = std::make_shared<SkyDome>();	// スカイドーム
	}

	// ライトの設定
	{
		auto& app = Application::GetInstance();
		// ライティングを使用する
		SetUseLighting(true);

		// ライトのカラーを調整する
		SetLightDifColor(GetColorF(app.GetConstantFloat("LIGHT_COLOR_R"), app.GetConstantFloat("LIGHT_COLOR_G"), app.GetConstantFloat("LIGHT_COLOR_B"), app.GetConstantFloat("LIGHT_COLOR_ALPHA")));

		// ライトの角度を設定
		SetLightDirection(VECTOR{ app.GetConstantFloat("LIGHT_DIRECTION_X"), app.GetConstantFloat("LIGHT_DIRECTION_Y") ,app.GetConstantFloat("LIGHT_DIRECTION_Z"), });
	}

	// 画像のロード
	{
		// ガイド文章の画像のロード
		_guideHandle[GUIDE_CAMERA] = LoadGraph("data/image/GuideCamera.png");
		_guideHandle[GUIDE_MOVE] = LoadGraph("data/image/GuideMove.png");
		_guideHandle[GUIDE_JUMP] = LoadGraph("data/image/GuideJump.png");
		_guideHandle[GUIDE_SHOT] = LoadGraph("data/image/GuideShot.png");
		_guideHandle[GUIDE_BOMB] = LoadGraph("data/image/GuideBomb.png");
		_guideHandle[GUIDE_GRAPPLE] = LoadGraph("data/image/GuideGrapple.png");
		_guideHandle[GUIDE_END] = LoadGraph("data/image/GuideEnd.png");
		_guideHandle[GUIDE_RETURN] = LoadGraph("data/image/GuideReturn.png");


		// クリア画像のロード
		_clearHandle = LoadGraph("data/image/Clear.png");

		// ゲーム説明の画像ロード
		_explainHandle[0] = LoadGraph("data/image/Story1.png");
		_explainHandle[1] = LoadGraph("data/image/Story2.png");
		_explainHandle[2] = LoadGraph("data/image/Story3.png");
		_explainHandle[3] = LoadGraph("data/image/Story4.png");
		_explainHandle[4] = LoadGraph("data/image/Story5.png");
		_explainHandle[5] = LoadGraph("data/image/Story6.png");
	}

	// ガイド文章の座標の設定
	_guidePos = Vec2{ Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2, GetConstantInt("GUIDE_POS_Y") };

	// クリア画像の座標の設定
	_clearPos = Vec2{ Application::GetInstance().GetConstantInt("SCREEN_WIDTH") / 2, Application::GetInstance().GetConstantInt("SCREEN_HEIGHT") / 2 };

	// 関数ポインタの初期化
	_updateFunc = &TutorialManager::FirstUpdate;
	_drawFunc = &TutorialManager::FirstDraw;

	// 禊虫の更新
	_pWedgewormManager->Update();

	// プレイヤーの更新処理
	_pPlayerManager->Update();
}

TutorialManager::~TutorialManager()
{
	// ガイド文章の削除
	for (int i = 0; i < 5; i++) {
		DeleteGraph(_guideHandle[i]);
	}

	// クリア画像の削除
	DeleteGraph(_clearHandle);

	// ゲーム説明の画像の削除
	for (int i = 0; i < 7; i++) {
		DeleteGraph(_explainHandle[i]);
	}
}

void TutorialManager::Update()
{
	// デフォルトの処理
	DefaultUpdate();

	(this->*_updateFunc)();
}

void TutorialManager::Draw() const
{
	(this->*_drawFunc)();
}

bool TutorialManager::GetEndFrag() const
{
	return _finishFrag;
}

void TutorialManager::FirstUpdate()
{
	// フレームの加算
	_frame++;

	if (7 * MAX_STORY_FRAME < _frame) {

		// 関数ポインタの初期化
		_updateFunc = &TutorialManager::CameraUpdate;
		_drawFunc = &TutorialManager::ShareDraw;
	}
}

void TutorialManager::CameraUpdate()
{
	if (_endFrag) {
		if (ClearFunction()) {
			_frame = 0;
			_endFrag = false;

			// チュートリアルを進める
			_updateFunc = &TutorialManager::MoveUpdate;
			_tutorialProgress++;
		}
	}
	// カメラ操作が１秒以上行われたらクリア処理を行う
	else if (Input::GetInstance().GetStickVectorLength(INPUT_RIGHT_STICK, INPUT_PAD_1) > GetConstantFloat("STICK_DEAD_ZONE")) {

		_frame++;
		if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM")) {
			_endFrag = true;
			_frame = 0;
			_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));
		}
	}
}

void TutorialManager::MoveUpdate()
{
	if (_endFrag) {
		if (ClearFunction()) {
			_frame = 0;
			_endFrag = false;

			// チュートリアルを進める
			_updateFunc = &TutorialManager::JumpUpdate;
			_tutorialProgress++;
		}
	}
	// 移動操作が１ち秒以上行われたらクリア処理を行う
	else if (Input::GetInstance().GetStickVectorLength(INPUT_LEFT_STICK, INPUT_PAD_1) > GetConstantFloat("STICK_DEAD_ZONE")) {

		_frame++;
		if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM")) {
			_endFrag = true;
			_frame = 0;
			_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));
		}
	}
}

void TutorialManager::JumpUpdate()
{
	if (_endFrag) {
		if (ClearFunction()) {
			_frame = 0;
			_endFrag = false;

			// チュートリアルを進める
			_updateFunc = &TutorialManager::ShotUpdate;
			_tutorialProgress++;
		}
	}
	// ジャンプ操作が一回以上行われたらクリア処理を行う
	else if (Input::GetInstance().IsTrigger(INPUT_A, INPUT_PAD_1)) {
		_endFrag = true;
		_frame = 0;
		_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));
	}
}

void TutorialManager::ShotUpdate()
{
	if (_endFrag) {
		if (ClearFunction()) {
			_frame = 0;
			_endFrag = false;

			_shotCount = 0;

			// チュートリアルを進める
			_updateFunc = &TutorialManager::BombUpdate;
			_tutorialProgress++;
		}
	}
	// 射撃操作が一回以上行われたらクリア処理を行う
	else if (Input::GetInstance().IsHold(INPUT_RIGHT_TRIGGER, INPUT_PAD_1)) {
		_shotCount++;
	}

	if (_shotCount == Application::GetInstance().GetConstantInt("FRAME_NUM")) {
		_endFrag = true;
		_frame = 0;
		_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));
		_shotCount = 0;
	}
}

void TutorialManager::BombUpdate()
{
	if (_endFrag) {
		if (ClearFunction()) {
			_frame = 0;
			_endFrag = false;
			_shotCount = 0;

			// チュートリアルを進める
			_updateFunc = &TutorialManager::GrappleUpdate;
			_tutorialProgress++;
		}
	}
	// 爆弾操作が一回以上行われたらクリア処理を行う
	else if (Input::GetInstance().IsTrigger(INPUT_Y, INPUT_PAD_1)) {
		_shotCount++;
	}

	if (_shotCount == GetConstantInt("BOMB_COUNT")) {
		_endFrag = true;
		_frame = 0;
		_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));
		_shotCount = 0;
	}
}

void TutorialManager::GrappleUpdate()
{
	if (_endFrag) {
		if (ClearFunction()) {
			_frame = 0;
			_endFrag = false;

			// チュートリアルを進める
			_updateFunc = &TutorialManager::LastUpdate;
			_drawFunc = &TutorialManager::LastDraw;
			_tutorialProgress++;
		}
	}
	// グラップル操作が一回以上行われたらクリア処理を行う
	else if (Input::GetInstance().IsTrigger(INPUT_X, INPUT_PAD_1)) {
		_endFrag = true;
		_frame = 0;
		_clearScale = static_cast<double>(GetConstantFloat("CLEAR_SCALE"));
	}
}

void TutorialManager::LastUpdate()
{
	if (_frame < 240) {
		_lastDrawGraph = _guideHandle[GUIDE_END];
		_frame++;
	}
	else {
		_lastDrawGraph = _guideHandle[GUIDE_RETURN];

		// スタートボタンでセレクト画面に戻る
		if (Input::GetInstance().IsTrigger(INPUT_START, INPUT_PAD_1)) {
			_finishFrag = true;
		}
	}
}

void TutorialManager::FirstDraw() const
{

	// 背景を少し暗くする
	SetDrawBlendMode(DX_BLENDMODE_MULA, 0);
	DrawBox(0, 0, Application::GetInstance().GetConstantInt("SCREEN_WIDTH"), Application::GetInstance().GetConstantInt("SCREEN_HEIGHT"), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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

	// エフェクトの描画
	EffectManager::GetInstance().Draw();

	// プレイヤーの描画
	_pPlayerManager->Draw(0);

	// 説明文の描画
	DrawRotaGraph(_guidePos.intX(), _guidePos.intY(), 1.0, 0.0, _explainHandle[static_cast<int>(_frame / MAX_STORY_FRAME)], true);
}

void TutorialManager::ShareDraw() const
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

	// エフェクトの描画
	EffectManager::GetInstance().Draw();

	// プレイヤーの描画
	_pPlayerManager->Draw(0);

	// ガイド文章の描画
	DrawRotaGraph(_guidePos.intX(), _guidePos.intY(), 1.0, 0.0, _guideHandle[_tutorialProgress], true);

	// クリア画像の描画
	if (_endFrag) DrawRotaGraph(_clearPos.intX(), _clearPos.intY(), _clearScale, 0.0, _clearHandle, true);
}

void TutorialManager::LastDraw() const
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

	// エフェクトの描画
	EffectManager::GetInstance().Draw();

	// プレイヤーの描画
	_pPlayerManager->Draw(0);

	// 文章の描画
	DrawRotaGraph(_guidePos.intX(), _guidePos.intY(), 1.0, 0.0, _lastDrawGraph, true);
}

bool TutorialManager::ClearFunction()
{
	auto& app = Application::GetInstance();

	_clearScale -= static_cast<double>(GetConstantFloat("CLEAR_SCALE_RATE"));

	_clearScale = max(1.0, _clearScale);

	if (_clearScale == 1.0 && _frame < 30) {
		_clearPos.x = app.GetConstantInt("SCREEN_WIDTH") / static_cast<float>(2) + GetRand(20) - 10;
		_clearPos.y = app.GetConstantInt("SCREEN_HEIGHT") / static_cast<float>(2) + GetRand(20) - 10;
	}

	if (_frame > 30) {
		_clearPos = Vec2{ app.GetConstantInt("SCREEN_WIDTH") / 2,app.GetConstantInt("SCREEN_HEIGHT") / 2 };
	}

	_frame++;
	if (_frame > 90) {
		return true;
	}
	else {
		return false;
	}
}

void TutorialManager::DefaultUpdate()
{
	// スカイドームの更新処理
	_pSkyDome->Update();

	// プレイヤーの更新処理
	_pPlayerManager->Update();

	// バレットの更新
	_pBulletManager->Update();

	// 禊虫の更新
	_pWedgewormManager->Update();
}
