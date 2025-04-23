#pragma once
#include "Components.h"
#include <memory>
#include "Constant.h"
#include "Vec2.h"
#include "PlayerManager.h"

class MyEffect;
class BulletManager;
class PlayerCamera;
class Player:
	public Transform,
	public CapsuleCollision,
	public Model,
	public Animation
{
public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="bullet"></param>
	/// <param name="manager"></param>
	/// <param name="padNum"></param>
	/// <param name="data"></param>
	Player(std::shared_ptr<BulletManager>& bullet, PlayerManager& manager, int padNum,BulletData& data);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="bullet"></param>
	/// <param name="manager"></param>
	/// <param name="data"></param>
	Player(std::shared_ptr<BulletManager>& bullet, PlayerManager& manager, BulletData& data);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Control();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// カメラの座標をセットする
	/// </summary>
	void CameraSet() const;

	/// <summary>
	/// 地上フラグを入手する
	/// </summary>
	/// <returns>グラウンドフラグ<returns>
	bool GetGroundFlag() const;

	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetDeadFlag() const;

	/// <summary>
	/// プレイヤーマネージャーからプレイヤーを殺す
	/// </summary>
	void KillPlayer();

	/// <summary>
	/// 自身のプレイヤーナンバーを返す
	/// </summary>
	/// <returns>プレイヤーナンバー</returns>
	int GetPlayerNum() const;

	/// <summary>
	/// 弾が当たった時に呼ばれる関数
	/// </summary>
	/// <param name="bul">弾の種類</param>
	void BulletCollision(int bul);

	/// <summary>
	/// スタンフラグを取得する
	/// </summary>:スタン中
	/// <returns>true</returns>
	bool GetStunFlag() const;


	void GiveItem(int itemType);

private:

	// コントロール関数ポインタ
	using m_controlFunc_t = void (Player::*)();
	m_controlFunc_t _controlFunc = nullptr;

	// コントロール関数ポインタ
	using m_updateFunc_t = void (Player::*)();
	m_updateFunc_t _updateFunc = nullptr;

	/// <summary>
	/// 移動などの処理
	/// </summary>
	void ControlPl();

	/// <summary>
	/// 操作AI
	/// </summary>
	void ControlAI();

	/// <summary>
	/// 更新処理
	/// </summary>
	void UpdatePl();

	/// <summary>
	/// AI更新処理
	/// </summary>
	void UpdateAI();

	/// <summary>
	/// Y軸の回転値を引数の値に徐々に近づけていく
	/// </summary>
	/// <param name="targetAngle">目標の値</param>
	void RotateAngleY(float targetAngle);

	/// <summary>
	/// 2次元ベクトルを角度ごとに8つに分類する
	/// </summary>
	/// <returns>分類先</returns>
	int ClassifyDirection();

	/// <summary>
	/// アニメーション制御関数
	/// </summary>
	void AnimationContorol();

	/// <summary>
	/// 選択した弾を発射する
	/// </summary>
	
	/// <summary>
	/// 引数の弾を発射する
	/// </summary>
	/// <param name="bullet">弾の種類</param>
	void BulletTrigger(int bullet);

	// Y軸の移動量
	float _moveScaleY;

	// 地上に立っているフラグ
	bool _groundFlag;

	// 前フレームの座標
	Vec3 _frontPos;

	// 移動ベクトル
	Vec3 _moveVec;

	// Y軸が一定になったカウント
	int _groundCount;

	// バレットマネージャーの参照
	std::shared_ptr<BulletManager>& _bulletManager;

	// カメラポインタ
	std::shared_ptr<PlayerCamera> _pCamera;

	// 使用するパッドのナンバー
	int _padNum;

	// プレイヤーマネージャーの参照
	PlayerManager& _manager;

	// 向いている方向のベクトル
	Vec3 _forwardVec;

	// グラップラー移動用単位ベクトル
	Vec3 _grapplerUnitVec;

	// グラップラー用移動量
	float _grapplerScale;

	// 死亡フラグ
	bool _deadFlag;

	// バレットデータの参照
	BulletData& _bulletData;

	// AI用のフレームカウンタ
	int _frame;

	// AI用の60フレーム前の座標
	Vec3 _oldPos;

	// エフェクト
	std::shared_ptr<MyEffect> _pEffect;

	// スタンフラグ
	bool _stunFrag;

	// スタンフレーム
	int _stunFrame;

	// アイテムによるスピードアップフラグ
	bool _speedUpFrag;

	// スピードアップフレーム
	int _speedUpFrame;
};

