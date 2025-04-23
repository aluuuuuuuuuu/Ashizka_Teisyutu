#pragma once
#include "Components.h"

class SkyDome :
	public Transform,
	public Model
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkyDome();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SkyDome();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
private:

};

