#pragma once
#include "Constant.h"
#include "Vec3.h"
#include "Components.h"

class PlayerCamera :
	public Constant,
	public Transform
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">プレイヤーの座標</param>
	/// <param name="padNum">パッドナンバー</param>
	/// <param name="forward">プレイヤーの方向ベクトル</param>
	PlayerCamera(Vec3 pos, int padNum, Vec3 forward);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerCamera();
	
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pos">プレイヤーの座標</param>
	/// <param name="forward">プレイヤーの方向ベクトル</param>
	void Update(Vec3 pos, Vec3 forward, Vec3 angle);

	/// <summary>
	/// ターゲットの座標を取得する
	/// </summary>
	/// <returns>ターゲットの座標</returns>
	Vec3 GetTarget();


private:

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="pos">プレイヤーの座標</param>
	/// <returns>回転後の座標</returns>
	/// <param name="forward">プレイヤーの方向ベクトル</param>
	Vec3 Rotate(Vec3 pos, Vec3 forward);


	/// <summary>
	/// 引数の座標を回転させる
	/// </summary>
	/// <param name="pos">プレイヤーの座標</param>
	/// <returns>回転後の座標</returns> 
	Vec3 RotatePos(Vec3 pos);

	Vec3 _target;

	// カメラから発せられるライトハンドル
	int _lightHandle;

	int _padNum;
};

