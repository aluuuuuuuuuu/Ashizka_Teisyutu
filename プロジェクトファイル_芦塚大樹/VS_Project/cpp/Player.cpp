#include "Player.h"
#include "DxLib.h"
#include "Input.h"
#include "PlayerCamera.h"
#include "NormalBullet.h"
#include "BulletManager.h"
#include "BulletBase.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "MyEffect.h"
#include "SoundManager.h"
#include "ItemManager.h"
#include "Application.h"
#include "Constant.h"

Player::Player(std::shared_ptr<BulletManager>& bullet, PlayerManager& manager, int padNum, BulletData& data) :
	_moveScaleY(0),
	_groundFlag(false),
	_bulletManager(bullet),
	_groundCount(0),
	_padNum(padNum),
	_manager(manager),
	_grapplerScale(0),
	_deadFlag(false),
	_bulletData(data),
	_frame(0),
	_stunFrag(false),
	_stunFrame(0),
	_speedUpFrag(false),
	_speedUpFrame(0)
{
	// 関数ポインタの初期化
	_controlFunc = &Player::ControlPl;
	_updateFunc = &Player::UpdatePl;

	// 拡大の設定
	Scale = manager.GetConstantFloat("MODEL_SCALE");

	// モデルの初期処理
	InitModel(_manager.GetModelHandle(padNum));

	// 座標の設定
	Position = Vec3{ 0.0f,25.0f,0.0f };

	// カプセルの初期化
	InitCapsule(Position, _manager.GetConstantFloat("CAPSULE_RADIUS"), _manager.GetConstantFloat("CAPSULE_HEIGHT"));

	// アニメーションの初期処理
	InitAnimation(_modelHandle, _manager.GetConstantInt("ANIM_AIMING_IDLE"), _manager.GetConstantFloat("BLEND_RATE"));

	// カメラの作成
	_pCamera = std::make_shared<PlayerCamera>(Position, _padNum, _forwardVec);

	// バレットデータの初期化
	// クールタイムの初期化
	for (auto& time : _bulletData._bullletCoolTime) {
		time = 0;
	}

	// エフェクトインスタンスの作成
	_pEffect = std::make_shared<MyEffect>(SPEED_UP_EFFECT, Position);

	// 選択している弾の初期化
	_bulletData._selectBullet = NORMAL_BULLET;
}

Player::Player(std::shared_ptr<BulletManager>& bullet, PlayerManager& manager, BulletData& data) :
	_moveScaleY(0),
	_groundFlag(false),
	_bulletManager(bullet),
	_groundCount(0),
	_manager(manager),
	_grapplerScale(0),
	_deadFlag(false),
	_bulletData(data),
	_frame(0),
	_padNum(1),
	_stunFrag(false),
	_stunFrame(0),
	_speedUpFrag(false),
	_speedUpFrame(0)
{
	// 関数ポインタの初期化
	_controlFunc = &Player::ControlAI;
	_updateFunc = &Player::UpdateAI;

	// 乱数生成器の初期化
	srand(static_cast<unsigned int>(time(nullptr)));

	// 拡大の設定
	Scale = manager.GetConstantFloat("MODEL_SCALE");

	// モデルの初期処理
	InitModel(_manager.GetModelHandle(1));

	// 座標の設定
	Position = Vec3{ 0.0f,25.0f,0.0f };

	// カプセルの初期化
	InitCapsule(Position, _manager.GetConstantFloat("CAPSULE_RADIUS"), _manager.GetConstantFloat("CAPSULE_HEIGHT"));

	// アニメーションの初期処理
	InitAnimation(_modelHandle, _manager.GetConstantInt("ANIM_AIMING_IDLE"), _manager.GetConstantFloat("BLEND_RATE"));

	// バレットデータの初期化
	// クールタイムの初期化
	for (auto& time : _bulletData._bullletCoolTime) {
		time = 0;
	}

	// エフェクトインスタンスの作成
	_pEffect = std::make_shared<MyEffect>(SPEED_UP_EFFECT, Position);

	// 選択している弾の初期化
	_bulletData._selectBullet = NORMAL_BULLET;

	_oldPos = Position;
}

Player::~Player()
{
}

void Player::Control()
{
	(this->*_controlFunc)();
}

void Player::Update()
{
	// スタンフラグの計測
	if (_stunFrag) {
		_stunFrame++;
		if (_stunFrame > _manager.GetConstantInt("STUN_TIME")) {
			_stunFrame = 0;
			_stunFrag = false;
		}
	}

	// スピードフラグのフレームを計測する
	if (_speedUpFrag) {
		_pEffect->Update(Position);
		_speedUpFrame++;
		if (_speedUpFrame > _manager.GetConstantInt("SPEED_UP_TIME")) {
			_pEffect->StopEffect();
			_speedUpFrag = false;
			_speedUpFrame = 0;
		}
	}

	(this->*_updateFunc)();
}

void Player::ControlPl()
{
	// インプットのインスタンスを取得
	auto& input = Input::GetInstance();

	// 通常弾を発射
	if (input.IsHold(INPUT_RIGHT_TRIGGER, _padNum)) {
		BulletTrigger(NORMAL_BULLET);
	}

	// 特殊弾の発射	
	if (input.IsHold(INPUT_X, _padNum)) {		// グラップル
		BulletTrigger(GRAPPLER_BULLET);
	}
	else if (input.IsHold(INPUT_Y, _padNum)) {	// 爆弾
		BulletTrigger(BOMB_BULLET);
	}

	// クールタイムの計算
	if (_bulletData._bullletCoolTime[NORMAL_BULLET] != 0) {
		_bulletData._bullletCoolTime[NORMAL_BULLET]--;
	}
	else {
		_bulletData._bullletCoolTime[NORMAL_BULLET] = 0;
	}
	if (_bulletData._bullletCoolTime[GRAPPLER_BULLET] != 0) {
		_bulletData._bullletCoolTime[GRAPPLER_BULLET]--;
	}
	else {
		_bulletData._bullletCoolTime[GRAPPLER_BULLET] = 0;
	}
	if (_bulletData._bullletCoolTime[BOMB_BULLET] != 0) {
		_bulletData._bullletCoolTime[BOMB_BULLET]--;
	}
	else {
		_bulletData._bullletCoolTime[BOMB_BULLET] = 0;
	}

	// 右スティックで回転
	if (input.GetStickVectorLength(INPUT_RIGHT_STICK, _padNum) > _manager.GetConstantFloat("STICK_DEAD_ZONE")) {

		// スティックを傾けた方向の回転の値を増減させる
		if (input.GetStickVector(INPUT_RIGHT_STICK, _padNum).x != 0) {
			Angle.y += _manager.GetConstantFloat("ANGLE_SCALE") * (input.GetStickThumbX(INPUT_RIGHT_STICK, _padNum));

			// ラジアン角を正規化する
			Angle.y = fmodf(Angle.y, static_cast<float>(DX_TWO_PI));
			if (Angle.y < 0.0f) Angle.y += static_cast<float>(DX_TWO_PI);
		}
		if (input.GetStickVector(INPUT_RIGHT_STICK, _padNum).z != 0) {
			Angle.z += _manager.GetConstantFloat("ANGLE_SCALE") * (input.GetStickThumbY(INPUT_RIGHT_STICK, _padNum));

			// 最大値と最低値を調整する
			if (Angle.z <= -_manager.GetConstantFloat("MAX_ANGLE")) {
				Angle.z = -_manager.GetConstantFloat("MAX_ANGLE");
			}
			else if (Angle.z >= _manager.GetConstantFloat("MAX_ANGLE")) {
				Angle.z = _manager.GetConstantFloat("MAX_ANGLE");
			}
		}
	}

	// コリジョン前の移動

	// 移動ベクトルの初期化
	_moveVec = 0;

	// スティックの入力値を移動ベクトルに代入する
	if (Input::GetInstance().GetStickVectorLength(INPUT_LEFT_STICK, _padNum) > _manager.GetConstantFloat("STICK_DEAD_ZONE")) {
		_moveVec = Input::GetInstance().GetStickUnitVector(INPUT_LEFT_STICK, _padNum);

		// 単位ベクトルの方向に移動速度分移動するベクトルを作成する
		if (_stunFrag) {
			_moveVec = _moveVec * _manager.GetConstantFloat("STUN_WALK_SPEED");
		}
		else {

			if (_speedUpFrag) {
				_moveVec = _moveVec * _manager.GetConstantFloat("SPEED_UP_WALK");
			}
			else {
				_moveVec = _moveVec * _manager.GetConstantFloat("WALK_SPEED");
			}
		}
	}

	// Aボタンでジャンプ
	if (Input::GetInstance().IsTrigger(INPUT_A, _padNum) && _groundFlag) {

		// ジャンプ力を与える
		_moveScaleY = _manager.GetConstantFloat("JUMP_SCALE");

		// ジャンプの開始アニメーションを再生
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_JUMP_UP"), false, _manager.GetConstantFloat("BLEND_RATE"));
	}

	// y軸の移動も足す
	_moveVec.y += _moveScaleY;

	// 移動ベクトルをy軸回転させる

	// 角度のずれを修正する
	float angle = Angle.y - DX_PI_F / 2;

	// Y軸回転行列に変換
	MATRIX rotaMtx = MGetRotY(angle);

	// スティック入力は逆になるから修正する
	_moveVec = Vec3{ _moveVec.x * -1, _moveVec.y, _moveVec.z * -1 };

	// 移動ベクトルを回転値に合わせてY軸回転させる
	_moveVec = VTransform(_moveVec.VGet(), rotaMtx);

	// グラップラーの移動
	_moveVec += _grapplerUnitVec * _grapplerScale;

	// 座標に移動ベクトルを足す
	Position += _moveVec;

	// カプセルに座標を渡す
	Set(Position);
}

void Player::ControlAI()
{
	// 移動ベクトルの初期化
	_moveVec = 0;

	// クールタイムの計算
	if (_bulletData._bullletCoolTime[NORMAL_BULLET] != 0) {
		_bulletData._bullletCoolTime[NORMAL_BULLET]--;
	}
	else {
		_bulletData._bullletCoolTime[NORMAL_BULLET] = 0;
	}
	if (_bulletData._bullletCoolTime[GRAPPLER_BULLET] != 0) {
		_bulletData._bullletCoolTime[GRAPPLER_BULLET]--;
	}
	else {
		_bulletData._bullletCoolTime[GRAPPLER_BULLET] = 0;
	}
	if (_bulletData._bullletCoolTime[BOMB_BULLET] != 0) {
		_bulletData._bullletCoolTime[BOMB_BULLET]--;
	}
	else {
		_bulletData._bullletCoolTime[BOMB_BULLET] = 0;
	}

	// AI処理
	{
		// 常にプレイヤーの方向を向く
		Vec3 targetPos = _manager.GetPlayerPos();	// プレイヤーの座標
		Vec3 length = targetPos - Position;	// プレイヤーとの距離
		Vec3 dist = (targetPos - Position).GetNormalized();	// プレイヤーの方向への単位ベクトル
		Angle.y = atan2(dist.x, dist.z) - DX_PI_F / 2;	// プレイヤーの方向を向く 

		// forwardVecがプレイヤーの足元の少し下を向くようにする
		_forwardVec = (Vec3{ targetPos.x,0.0f,targetPos.z } - Position).GetNormalized();

		// グラップルが着弾していたらプレイヤーの方法に移動する
		if (_bulletManager->IsCollisionBullet(_padNum) && !_bulletManager->GetInvalidFlag(_padNum)) {
			_moveVec += dist * _manager.GetConstantFloat("STUN_WALK_SPEED") * 1.5f;
		}

		// ランダムな間隔でジャンプする
		if (rand() % _manager.GetConstantInt("JUMP_INTERVAL") == 0 && _groundFlag) {
			_moveScaleY = _manager.GetConstantFloat("JUMP_SCALE");
		}

		if (_frame % Application::GetInstance().GetConstantInt("FRAME_NUM") == 0) {
			if ((Position - _oldPos).Length() < 10.0f) {
				_moveScaleY = _manager.GetConstantFloat("JUMP_SCALE");
			}
			_oldPos = Position;
		}

		if (_stunFrag) {
			_moveVec += dist * _manager.GetConstantFloat("STUN_WALK_SPEED");
		}
		else {
			if (_speedUpFrag) {
				_moveVec += dist * _manager.GetConstantFloat("SPEED_UP_WALK");
			}
			else {
				_moveVec += dist * _manager.GetConstantFloat("WALK_SPEED");
			}
		}

		// 30フレームに一回弾を発射する
		if (_frame % Application::GetInstance().GetConstantInt("FRAME_NUM") / 2 == 0) {
			BulletTrigger(rand() % 3);
		}
	}

	// y軸の移動も足す
	_moveVec.y += _moveScaleY;

	// グラップラーの移動
	_moveVec += _grapplerUnitVec * _grapplerScale;

	// 座標に移動ベクトルを足す
	Position += _moveVec;

	// カプセルに座標を渡す
	Set(Position);

}

void Player::UpdatePl()
{
	// カプセルに座標を渡す
	Set(Position);

	// 前のy座標と今のy座標が同じであれば着地している
	if (_frontPos.y == Position.y) {
		if (_groundCount == 0) {
			_groundCount++;
		}
		else {
			_groundFlag = true;
		}
	}
	else {
		_groundCount = 0;
		_groundFlag = false;
	}

	// 前フレームの座標を保存する
	_frontPos = Position;

	// 落下速度を少しづつ早くする
	if (!_groundFlag && _moveScaleY > -_manager.GetConstantFloat("MAX_FALL_SPEED")) {
		_moveScaleY -= _manager.GetConstantFloat("FALL_SPEED");
	}

	// 向いている方向のベクトルを求める
	_forwardVec.x = std::cos(Angle.y * -1) * std::cos(Angle.z); // X成分
	_forwardVec.y = std::sin(Angle.z);                 // Y成分（上下方向の影響）
	_forwardVec.z = std::sin(Angle.y * -1) * std::cos(Angle.z); // Z成分

	// グラップラーの移動処理
	if (_groundFlag) {
		_grapplerScale = 0.0f;
	}
	else if (_grapplerScale <= 0.0f) {
		_grapplerScale = 0.0f;
	}
	else {
		_grapplerScale -= _manager.GetConstantFloat("GRAPPLER_DECREASE_SCALE");
	}

	// グラップラーが当たっているかを知らべる
	if (_bulletManager->IsCollisionBullet(_padNum) && !_bulletManager->GetInvalidFlag(_padNum)) {

		// 弾の無効化
		_bulletManager->KillBullet(_padNum);

		// 当たっていたら上方向とグラップラーの方向に移動ベクトルを与える
		if (_groundFlag) {
			_moveScaleY = _manager.GetConstantFloat("GRAPPLER_JUMP_SCALE_GROUND");
		}
		else {
			_moveScaleY = _manager.GetConstantFloat("GRAPPLER_JUMP_SCALE");
		}

		// グラップラーに向かう単位ベクトルを作成する
		_grapplerUnitVec = (_bulletManager->GetBulletPos(_padNum) - Position).GetNormalized();

		// グラップラーの着弾点への距離によって移動速度を変化させる
		_grapplerScale = _manager.GetConstantFloat("GRAPPER_SPEED_BASE") * (_bulletManager->GetBulletPos(_padNum) - Position).Length();

		// グラップラーによる移動速度の最大値を決める
		if (_grapplerScale > _manager.GetConstantFloat("GRAPPLER_MAX_SPEED")) {
			_grapplerScale = _manager.GetConstantFloat("GRAPPLER_MAX_SPEED");
		}
	}

	// カメラの更新
	_pCamera->Update(Position, _forwardVec, Angle);

	// アニメーションコントロール                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
	AnimationContorol();

	// アニメーションの更新
	if (_speedUpFrag) {
		UpdateAnimation(_modelHandle, _manager.GetConstantFloat("ANIM_SPEED_UP_WALK"));
	}
	else {
		UpdateAnimation(_modelHandle, _manager.GetConstantFloat("ANIM_SPEED_WALK"));
	}

	// モデル用のトランスフォームを作成する
	Transform trans;
	trans.Scale = Scale;
	trans.Position = Position;
	trans.Angle = Vec3{ Angle.x,Angle.y - DX_PI_F / 2, Angle.z };

	// モデルの更新
	UpdateModel(trans);
}

void Player::UpdateAI()
{
	// フレームの加算
	_frame++;

	// カプセルに座標を渡す
	Set(Position);

	// 前のy座標と今のy座標が同じであれば着地している
	if (_frontPos.y == Position.y) {
		if (_groundCount == 0) {
			_groundCount++;
		}
		else {
			_groundFlag = true;
		}
	}
	else {
		_groundCount = 0;
		_groundFlag = false;
	}

	// 前フレームの座標を保存する
	_frontPos = Position;

	// 落下速度を少しづつ早くする
	if (!_groundFlag && _moveScaleY > -_manager.GetConstantFloat("MAX_FALL_SPEED")) {
		_moveScaleY -= _manager.GetConstantFloat("FALL_SPEED");
	}

	// グラップラーの移動処理
	if (_groundFlag) {
		_grapplerScale = 0.0f;
	}
	else if (_grapplerScale <= 0.0f) {
		_grapplerScale = 0.0f;
	}
	else {
		_grapplerScale -= 0.01f;
	}

	// グラップラーが当たっているかを知らべる
	if (_bulletManager->IsCollisionBullet(_padNum) && !_bulletManager->GetInvalidFlag(_padNum)) {

		// 弾の無効化
		_bulletManager->KillBullet(_padNum);

		// 当たっていたら上方向とグラップラーの方向に移動ベクトルを与える
		if (_groundFlag) {
			_moveScaleY = _manager.GetConstantFloat("GRAPPLER_JUMP_SCALE_GROUND");
		}
		else {
			_moveScaleY = _manager.GetConstantFloat("GRAPPLER_JUMP_SCALE");
		}

		// グラップラーに向かう単位ベクトルを作成する
		_grapplerUnitVec = (_bulletManager->GetBulletPos(_padNum) - Position).GetNormalized();

		// グラップラーの着弾点への距離によって移動速度を変化させる
		_grapplerScale = 0.02f * (_bulletManager->GetBulletPos(_padNum) - Position).Length();

		// グラップラーによる移動速度の最大値を決める
		if (_grapplerScale > _manager.GetConstantFloat("GRAPPLER_MAX_SPEED")) {
			_grapplerScale = _manager.GetConstantFloat("GRAPPLER_MAX_SPEED");
		}
	}

	// アニメーションコントロール                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
	AnimationContorol();

	// アニメーションの更新
	if (_speedUpFrag) {
		UpdateAnimation(_modelHandle, _manager.GetConstantFloat("ANIM_SPEED_UP_WALK"));
	}
	else {
		UpdateAnimation(_modelHandle, _manager.GetConstantFloat("ANIM_SPEED_WALK"));
	}

	// モデル用のトランスフォームを作成する
	Transform trans;
	trans.Scale = Scale;
	trans.Position = Position;
	trans.Angle = Vec3{ Angle.x,Angle.y - DX_PI_F / 2, Angle.z };

	// モデルの更新
	UpdateModel(trans);
}

void Player::Draw() const
{

#ifdef _DEBUG
	DrawCapsule();
	//DrawLine3D(Position.VGet(), (Position + _forwardVec * 20).VGet(), 0x00ffff);
	//DrawFormatString(10, 20, 0xff0000, "x:%f y:%f z:%f angleY:%f angleZ:%f", Position.x, Position.y, Position.z, Angle.y, Angle.z);
	//if (_groundFlag) {
	//	DrawString(10, 40, "OnGrround", 0xff0000);
	//}
	//DrawFormatString(10, 60, 0xff0000, "GrapplerScale:%f", _grapplerScale);
	//if (_selectBullet == NORMAL_BULLET) {
	//	DrawString(10, 80, "NormalBullet", 0xff0000);
	//}
	//else if (_selectBullet == GRAPPLER_BULLET) {
	//	DrawString(10, 80, "GrapplerBullet", 0xff0000);
	//}
	//else if (_selectBullet == BOMB_BULLET) {
	//	DrawString(10, 80, "bombBullet", 0xff0000);
	//}

	//if (_deadFlag) {
	//	DrawString(10, 100, "dead", 0xff0000);
	//}
#endif // DEBUG
	if (_bulletManager->GetBulletExist(_padNum)) {
		Vec3 pos = Vec3{ Position.x,Position.y + _manager.GetConstantFloat("GRAPPLE_MARGINE_Y"), Position.z };
		DrawLine3D(_bulletManager->GetBulletPos(_padNum).VGet(), pos.VGet(), 0xff16ff);
	}
	// モデルの描画
	DrawModel();
}

void Player::CameraSet() const
{
	// カメラのターゲットと座標を設定する
	SetCameraPositionAndTarget_UpVecY(_pCamera->Position.VGet(), _pCamera->GetTarget().VGet());
}

bool Player::GetGroundFlag() const
{
	return _groundFlag;
}

bool Player::GetDeadFlag() const
{
	return _deadFlag;
}

void Player::KillPlayer()
{
	_deadFlag = true;
}

int Player::GetPlayerNum() const
{
	return _padNum;
}

void Player::BulletCollision(int bul)
{
	// 弾の種類によって処理を変える
	switch (bul) {
	case NORMAL_BULLET:
		if (!_speedUpFrag) _stunFrag = true;
		break;
	case GRAPPLER_BULLET:
		break;
	case BOMB_BULLET:
		break;
	}
}

bool Player::GetStunFlag() const
{
	return _stunFrag;
}

void Player::GiveItem(int itemType)
{
	switch (itemType)
	{
	case ITEM_TYPE_SPEED:	// スピードアップ
		_pEffect = std::make_shared<MyEffect>(SPEED_UP_EFFECT, Position);

		if (_speedUpFrag) {
			_speedUpFrame = 0;
		}
		else {
			// スピードアップフラグを立てる
			_speedUpFrag = true;
		}
		break;
	default:
		break;
	}
}

void Player::RotateAngleY(float targetAngle)
{
	// 平行移動ベクトルが0じゃないときだけ角度を計算する
	if (_moveVec.x != 0.0f && _moveVec.z != 0.0f) {
		// 移動する方向に徐々に回転する

		// 差が移動量より小さくなったら目標の値を代入する
		if (fabsf(Angle.y - targetAngle) > _manager.GetConstantFloat("ANGLE_ROTATE_SCALE")) {
			// 増やすのと減らすのでどちらが近いか判断する
			float add = targetAngle - Angle.y;	// 足す場合の回転量
			if (add < 0.0f) add += static_cast<float>(DX_TWO_PI);	// 足す場合の回転量が負の数だった場合正規化する
			float sub = static_cast<float>(DX_TWO_PI) - add;	// 引く場合の回転量

			// 回転量を比べて少ない方を選択する
			if (add < sub) {
				Angle.y += _manager.GetConstantFloat("ANGLE_ROTATE_SCALE");
			}
			else {
				Angle.y -= _manager.GetConstantFloat("ANGLE_ROTATE_SCALE");
			}

			// 増減によって範囲外になった場合の正規化
			Angle.y = fmodf(Angle.y, static_cast<float>(DX_TWO_PI));
			if (Angle.y < 0.0f) Angle.y += static_cast<float>(DX_TWO_PI);
		}
		else {
			Angle.y = targetAngle;
		}
	}
}

int Player::ClassifyDirection()
{
	// キャラクターの向きに合わせて移動ベクトルを回転
	float rotatedX = _moveVec.x * cos(Angle.y) - _moveVec.z * sin(Angle.y);
	float rotatedZ = _moveVec.x * sin(Angle.y) + _moveVec.z * cos(Angle.y);

	float angle = atan2(rotatedX, rotatedZ);

	// 45度（π/4）ごとに方向を分類
	if (angle >= -DX_PI / 8 && angle < DX_PI / 8) {
		return 0;  // 前
	}
	else if (angle >= DX_PI / 8 && angle < 3 * DX_PI / 8) {
		return 1;  // 前右
	}
	else if (angle >= 3 * DX_PI / 8 && angle < 5 * DX_PI / 8) {
		return 2;  // 右
	}
	else if (angle >= 5 * DX_PI / 8 && angle < 7 * DX_PI / 8) {
		return 3;  // 後右
	}
	else if ((angle >= 7 * DX_PI / 8 && angle <= DX_PI) || (angle < -7 * DX_PI / 8 && angle >= -DX_PI)) {
		return 4;  // 後
	}
	else if (angle >= -7 * DX_PI / 8 && angle < -5 * DX_PI / 8) {
		return 5;  // 後左
	}
	else if (angle >= -5 * DX_PI / 8 && angle < -3 * DX_PI / 8) {
		return 6;  // 左
	}
	else if (angle >= -3 * DX_PI / 8 && angle < -DX_PI / 8) {
		return 7;  // 前左
	}

	return 0;  // デフォルトは前
}

void Player::AnimationContorol()
{
	// 地面についていないとき
	if (!_groundFlag) {
		// ジャンプアップアニメーション出なければループアニメーションを再生する
		if (GetAnimTag() != _manager.GetConstantInt("ANIM_JUMP_UP")) {
			ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_JUMP_LOOP"), true, _manager.GetConstantFloat("BLEND_RATE"));
		}
		return;
	}

	if (_moveVec.x == 0.0f && _moveVec.z == 0.0f) {
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_AIMING_IDLE"), true, _manager.GetConstantFloat("BLEND_RATE"));
		return;
	}

	int dir = ClassifyDirection();

	// 歩いている
	// 方向に対応するアニメーションを再生
	switch (dir) {
	case 0:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_LEFT"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 1:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_FORWARD_LEFT"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 2:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_FORWARD"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 3:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_FORWARD_RIGHT"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 4:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_RIGHT"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 5:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_BACKWARD_RIGHT"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 6:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_BACKWARD"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	case 7:
		ChangeAnimation(_modelHandle, _manager.GetConstantInt("ANIM_RUN_BACKWARD_LEFT"), true, _manager.GetConstantFloat("BLEND_RATE"));
		break;
	}
}

void Player::BulletTrigger(int bullet)
{
	// 発射する座標
	Vec3 pos = MV1GetFramePosition(GetModelHandle(), _manager.GetConstantInt("BONE_FINGER"));

	switch (bullet)
	{
	case NORMAL_BULLET:
		if (_bulletData._bullletCoolTime[NORMAL_BULLET] == 0) {

			// 通常弾発射音を鳴らす
			SoundManager::GetInstance().RingSE(SE_SHOT_NORMAL_BULET);

			// 弾を生成
			_bulletManager->PushBullet(NORMAL_BULLET, _forwardVec, pos, _padNum);

			// クールタイムを設定する
			_bulletData._bullletCoolTime[NORMAL_BULLET] = _manager.GetConstantInt("COOL_TIME_NORMAL");
		}
		break;
	case GRAPPLER_BULLET:
		if (_bulletData._bullletCoolTime[GRAPPLER_BULLET] == 0) {

			// 通常弾発射音を鳴らす
			SoundManager::GetInstance().RingSE(SE_SHOT_GRAPPLE_BULET);

			// 弾を生成
			_bulletManager->PushBullet(GRAPPLER_BULLET, _forwardVec, pos, _padNum);

			// クールタイムを設定する
			_bulletData._bullletCoolTime[GRAPPLER_BULLET] = _manager.GetConstantInt("COOL_TIME_GRAPPLER");
		}
		break;
	case BOMB_BULLET:
		if (_bulletData._bullletCoolTime[BOMB_BULLET] == 0) {

			// 通常弾発射音を鳴らす
			SoundManager::GetInstance().RingSE(SE_SHOT_BOMB_BULET);

			// 弾を生成
			_bulletManager->PushBullet(BOMB_BULLET, _forwardVec, pos, _padNum);

			// クールタイムを設定する
			_bulletData._bullletCoolTime[BOMB_BULLET] = _manager.GetConstantInt("COOL_TIME_BOMB");
		}
		break;
	default:
		break;
	}
}
