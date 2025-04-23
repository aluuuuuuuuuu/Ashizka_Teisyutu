#pragma once
#include "Components.h"
#include "Vec3.h"

class Wedgeworm:
	public Transform,
	public Model,
	public Animation
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="num">ナンバー</param>
	/// 
	
	Wedgeworm(int num,int model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Wedgeworm();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
private:
	float _angle;
	Vec3 _aa;

	MATRIX _mtx;

	Vec3 _pos;

};

