#pragma once
#include "Components.h"

class TitlePlayer:
	public Transform,
	public Model,
	public Animation
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">左右どちらか</param>
	/// <param name="num">プレイヤーナム</param>
	TitlePlayer(int pos,int num);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitlePlayer();

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

