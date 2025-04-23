#pragma once
#include "Vec2.h"

class MenuButton
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">表示中心座標</param>
	/// <param name="range">当たり判定範囲</param>
	/// <param name="handle">画像ハンドル</param>
	/// <param name="type">自身のボタンの種類</param>
	MenuButton(Vec2 pos, Vec2 range, int handle,int type);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MenuButton();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="margin">余白</param>
	void Draw(int margin) const;

	/// <summary>
	/// 当たり判定範囲を取得する
	/// </summary>
	/// <returns>範囲</returns>
	Vec2 GetRange() const;

	/// <summary>
	/// 座標を取得する
	/// </summary>
	/// <returns>座標</returns>
	Vec2 GetPos() const;

	/// <summary>
	/// 種類を取得する
	/// </summary>
	/// <returns>種類</returns>
	int GetType() const;

	/// <summary>
	/// 画像を差し替える
	/// </summary>
	/// <param name="handle">差し替える画像ハンドル</param>
	void ChangeImage(int handle);

private:

	// 画像ハンドル
	int _graphHandle;

	// 表示中心座標
	Vec2 _pos;

	// 当たり判定範囲
	Vec2 _range;

	// 自身の種類1
	int _type;
};

