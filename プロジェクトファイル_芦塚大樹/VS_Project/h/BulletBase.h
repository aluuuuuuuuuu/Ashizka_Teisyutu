#pragma once
#include "Vec3.h"
#include "DxLib.h"
#include "Components.h"
#include <memory>

class MyEffect;
class BulletManager;
class MapBulletCollisionManager;
class BulletBase:
	public Transform
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BulletBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BulletBase() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const = 0;

	/// <summary>
	/// 死亡フラグを返す
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool IsDead() const;

	 /// <summary>
	 /// 自身の弾の種類を返す
	 /// </summary>
	 /// <returns>バレットマネージャーの定数</returns>
	 int GetBulletType() const;

	 /// <summary>
	 /// マップと当たったかどうかを返す現状グラップラーのみ判定
	 /// </summary>
	 /// <returns>当たっていたらtrue</returns>
	 bool GetCollisionFlag() const;

	 /// <summary>
	 /// 弾が無効かされているか返す現状グラップラーのみ
	 /// </summary>
	 /// <returns>無効であればtrue</returns>
	 bool GetInvalidFlag() const;

	 /// <summary>
	 /// 弾を無効化する
	 /// </summary>
	 void KillBullet();

	 /// <summary>
	 /// 自身がどのプレイヤーから発射されたかを返す
	 /// </summary>
	 /// <returns>プレイヤーナンバー</returns>
	 int GetPlayerNum();

	 /// <summary>
	 /// 半径を取得する
	 /// </summary>
	 /// <returns>半径</returns>
	 float GetRadius() const;

	 /// <summary>
	 /// プレイヤーと当たった時の処理
	 /// </summary>
	 virtual void PlayerCollision() = 0;

	 /// <summary>
	 /// プレイヤーとの当たり判定フラグを返す
	 /// </summary>
	 /// <returns>trueで当たっている</returns>
	 bool GetPlayerCollisionFlag() const;

protected:

	// 死亡フラグ
	bool _deadFlag;

	// 自身の弾の種類
	int _bulletType;

	// 当たったフラグ
	bool _collisionFlag;

	// 無効フラグ
	bool _invalidFlag;

	// プレイヤーと当たったフラグ
	bool _playerCollisionFlag;

	// 自身がどのプレイヤーから発射されたかを保存しておく
	int _playerNum;

	// 移動方向ベクトル
	Vec3 _distVec;

	// 重力
	float _gravity;

	// エフェクトポインタ
	std::shared_ptr<MyEffect> _pEffect;

	// 半径
	float _radius;

};

