#pragma once
#include "Vec2.h"
#include "PlayerManager.h"

class PlayerUi
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerNum">プレイヤーの総数</param>
	PlayerUi(int playerNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerUi();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="center">描画の中心座標</param>
	void Draw(Vec2 center, BulletData data) const;
private:

	// 通常弾画像ハンドル
	int _normalBulletHandle;

	// 爆発弾画像ハンドル
	int _bombBulletHandle;

	// グラップル弾画像ハンドル
	int _grappleBulletHandle;

	// レティクル画像ハンドル
	int _reticleHandle;

	// 画面中心からの描画余白
	Vec2 _drawMargin;

	// アイコン同士の余白
	int _margin;

	// 画像の拡大率
	double _exRate;

	// フレームの描画座標
	Vec2 _framePos;

	// フレームカウンタ
	float _frame;

};

