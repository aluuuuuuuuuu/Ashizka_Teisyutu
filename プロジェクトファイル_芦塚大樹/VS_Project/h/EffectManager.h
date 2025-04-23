#pragma once
#include "Singleton.h"
#include <map>
#include "Vec3.h"
#include "Constant.h"

constexpr int NORMAL_BULLET_EFFECT = 0;
constexpr int BOMB_BULLET_EFFECT = 1;
constexpr int GRAPPLE_BULLET_EFFECT = 2;
constexpr int BLOCK_DESTROY_EFFECT = 3;
constexpr int JUMP_EFFECT = 4;
constexpr int WIN_EFFECT = 5;
constexpr int SHOT_DOWN_EFFECT = 6;
constexpr int SPEED_UP_EFFECT = 7;

constexpr int MAX_EFFECT_NUM = 8;

class EffectManager:
	public Singleton<EffectManager>,
	public Constant
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Terminate();

	/// <summary>
	/// エフェクトのハンドルを取得する
	/// </summary>
	/// <param name="effect">エフェクトのキー</param>
	/// <returns>ハンドル取得</returns>
	int& GetEffectHandle(int effect);

	/// <summary>
	/// すべてのエフェクトハンドルを削除する
	/// </summary>
	void DeleteEffect();

private:

	/// <summary>
	/// 指定されたエフェクトをロードする
	/// </summary>
	/// <param name="effect">エフェクトのキー</param>
	void LoadEffect(int effect);

	// エフェクトマップ
	std::map<int, int> _effectMap;
};

