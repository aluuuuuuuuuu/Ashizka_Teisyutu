#pragma once
#include "Vec3.h"

class MyEffect
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="effect">エフェクトキー</param>
	/// <param name="pos">初期位置</param>
	MyEffect(int effect, Vec3 pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MyEffect();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pos">座標</param>
	void Update(Vec3 pos);

	/// <summary>
	/// エフェクトを停止する
	/// </summary>
	void StopEffect();

	/// <summary>
	/// エフェクトを開始する
	/// </summary>
	void StartEffect();

	/// <summary>
	/// スケールをセットする
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScaleEffect(Vec3 scale);


private:

	// プレイハンドル
	int _playHandle;
};

