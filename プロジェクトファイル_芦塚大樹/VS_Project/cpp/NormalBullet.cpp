#include "NormalBullet.h"
#include "MapBulletCollisionManager.h"
#include "BulletManager.h"
#include "EffectManager.h"
#include "MyEffect.h"
#include "EffekseerForDXLib.h"
#include "SoundManager.h"
#include "Application.h"

NormalBullet::NormalBullet(Vec3 dist, Vec3 pos, std::shared_ptr<MapBulletCollisionManager>& col, BulletManager& mgr, int plNum) :
	_collManager(col),
	_bulletManager(mgr),
	_frame(0)
{
	// 初期位置の設定
	Position = pos;

	// バレットタイプの設定
	_bulletType = NORMAL_BULLET;

	// 発射したプレイヤーの保存
	_playerNum = plNum;

	// 進む方向
	_distVec = dist;

	// エフェクトインスタンスの作成
	_pEffect = std::make_shared<MyEffect>(NORMAL_BULLET_EFFECT, pos);

	// 半径の設定
	_radius = _bulletManager.GetConstantFloat("RADIUS");
}

NormalBullet::~NormalBullet()
{
	if (_frame == 0) {
		_pEffect->StopEffect();
	}
	else if (!_deadFlag) {
		_destroyEffect->StopEffect();
	}
}

void NormalBullet::Update()
{
	// ポジションの更新
	if (_frame == 0) {
		Position += _distVec * _bulletManager.GetConstantFloat("SPEED");
		// 重力の影響を与える
		_gravity += _bulletManager.GetConstantFloat("GRAVITY");
		Position.y -= _gravity;

		// マップとの当たり判定をとる
		if (_collManager->CollisionBullet(Position, _radius, NORMAL_BULLET)) {

			// 着弾音を鳴らす
			SoundManager::GetInstance().RingSE(SE_DESTRUCTION);

			// 当たった時の処理
			CollisionFunction();
		}

		// 一定のラインを越えたら削除する
		float deadLine = _bulletManager.GetConstantFloat("DEAD_LINE");
		if (abs(Position.x) >= deadLine || abs(Position.y) >= deadLine || abs(Position.z) >= deadLine) {
			_deadFlag = true;
			_pEffect->StopEffect();
		}
	}
	else {
		_destroyEffect->Update(Position);
		_frame++;
		if (_frame > Application::GetInstance().GetConstantInt("FRAME_NUM")) {
			_deadFlag = true;
			_destroyEffect->StopEffect();
		}
	}

	// エフェクトの更新
	_pEffect->Update(Position);
}

void NormalBullet::Draw() const
{
#ifdef _DEBUG
	//DrawSphere3D(Position.VGet(), 3.0f, 16, 0x0000ff, 0x0000ff, true);
#endif // DEBUG
}

void NormalBullet::PlayerCollision()
{
	CollisionFunction();
}

void NormalBullet::CollisionFunction()
{
	_playerCollisionFlag = true;
	_frame = 1;
	_pEffect->StopEffect();
	_destroyEffect = std::make_shared<MyEffect>(BLOCK_DESTROY_EFFECT, Position);
	_destroyEffect->SetScaleEffect(Vec3{ 0.1f,0.1f,0.1f });
}
