#include "AIEnemy.h"
#include "DxLib.h"
#include "Input.h"
#include "PlayerCamera.h"
#include "NormalBullet.h"
#include "BulletManager.h"
#include "BulletBase.h"
#include "PlayerManager.h"

AIEnemy::AIEnemy(std::shared_ptr<BulletManager>& bullet, BulletData& data) :
	_moveScaleY(0),
	_groundFlag(false),
	_bulletManager(bullet),
	_groundCount(0),
	_grapplerScale(0),
	_deadFlag(false),
	_bulletData(data)
{
	// 拡大の設定
	Scale = Vec3{ 0.12f,0.12f,0.12f };

	// モデルの初期処理
	InitModel(_manager.GetModelHandle(padNum));

	// 座標の設定
	Position = Vec3{ 0.0f,25.0f,0.0f };

	// カプセルの初期化
	InitCapsule(Position, 3.0f, 12);

	// アニメーションの初期処理
	InitAnimation(_modelHandle, _manager.GetConstantInt("ANIM_AIMING_IDLE"), _manager.GetConstantFloat("BLEND_RATE"));

	// バレットデータの初期化
	// クールタイムの初期化
	for (auto& time : _bulletData._bullletCoolTime) {
		time = 0;
	}

	// 選択している弾の初期化
	_bulletData._selectBullet = NORMAL_BULLET;
}

AIEnemy::~AIEnemy()
{
}

void AIEnemy::Control()
{
}

void AIEnemy::Update()
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
	if (!_groundFlag && _moveScaleY > -1.5f) {
		_moveScaleY -= 0.1f;
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
		_grapplerScale -= 0.01f;
	}

	// グラップラーが当たっているかを知らべる
	if (_bulletManager->IsCollisionBullet(_padNum) && !_bulletManager->GetInvalidFlag(_padNum)) {

		// 弾の無効化
		_bulletManager->KillBullet(_padNum);

		// 当たっていたら上方向とグラップラーの方向に移動ベクトルを与える
		if (_groundFlag) {
			_moveScaleY = 2.4f;
		}
		else {
			_moveScaleY = 1.5f;
		}

		// グラップラーに向かう単位ベクトルを作成する
		_grapplerUnitVec = (_bulletManager->GetBulletPos(_padNum) - Position).GetNormalized();

		// グラップラーの着弾点への距離によって移動速度を変化させる
		_grapplerScale = 0.02f * (_bulletManager->GetBulletPos(_padNum) - Position).Length();

		// グラップラーによる移動速度の最大値を決める
		if (_grapplerScale > 1.8f) {
			_grapplerScale = 1.8f;
		}
	}

	// アニメーションコントロール                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
	AnimationContorol();

	// アニメーションの更新
	UpdateAnimation(_modelHandle, _manager.GetConstantFloat("ANIM_SPEED_WALK"));

	// モデル用のトランスフォームを作成する
	Transform trans;
	trans.Scale = Scale;
	trans.Position = Position;
	trans.Angle = Vec3{ Angle.x,Angle.y - 1.5708f,Angle.z };

	// モデルの更新
	UpdateModel(trans);
}

void AIEnemy::Draw() const
{
	// モデルの描画
	DrawModel();
}

bool AIEnemy::GetGroundFlag() const
{
	return _groundFlag;
}

bool AIEnemy::GetDeadFlag() const
{
	return _deadFlag;
}

void AIEnemy::KillPlayer()
{
	_deadFlag = true;
}

void AIEnemy::RotateAngleY(float targetAngle)
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

int AIEnemy::ClassifyDirection()
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

void AIEnemy::AnimationContorol()
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

void AIEnemy::BulletTrigger()
{
	// 発射する座標
	Vec3 pos = { Position.x  ,Position.y + 10 ,Position.z };

	switch (_bulletData._selectBullet)
	{
	case NORMAL_BULLET:
		if (_bulletData._bullletCoolTime[NORMAL_BULLET] == 0) {
			// 弾を生成
			_bulletManager->PushBullet(NORMAL_BULLET, _forwardVec, pos, _padNum);

			// クールタイムを設定する
			_bulletData._bullletCoolTime[NORMAL_BULLET] = 30;
		}
		break;
	case GRAPPLER_BULLET:
		if (_bulletData._bullletCoolTime[GRAPPLER_BULLET] == 0) {
			// 弾を生成
			_bulletManager->PushBullet(GRAPPLER_BULLET, _forwardVec, pos, _padNum);

			// クールタイムを設定する
			_bulletData._bullletCoolTime[GRAPPLER_BULLET] = 1200;
		}
		break;
	case BOMB_BULLET:
		if (_bulletData._bullletCoolTime[BOMB_BULLET] == 0) {
			// 弾を生成
			_bulletManager->PushBullet(BOMB_BULLET, _forwardVec, pos, _padNum);

			// クールタイムを設定する
			_bulletData._bullletCoolTime[BOMB_BULLET] = 300;
		}
		break;
	default:
		break;
	}
}
