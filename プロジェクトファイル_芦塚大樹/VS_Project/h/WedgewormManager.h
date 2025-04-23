#pragma once
#include <memory>
#include "Vec3.h"

class Wedgeworm;
class WedgewormManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WedgewormManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~WedgewormManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 禊虫の座標を渡す
	/// </summary>
	/// <param name="num">禊虫ナンバー</param>
	/// <returns>座標</returns>
	Vec3 GetPos(int num) const;

private:

	// wormオブジェクトの配列
	std::shared_ptr<Wedgeworm> _worm[2];
};

