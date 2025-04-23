#pragma once
#include <memory>
#include "Vec2.h"

class MenuManager;
class MenuCollisionManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mane">メニューマネージャーのポインタ</param>
	MenuCollisionManager(MenuManager& mane);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MenuCollisionManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="fingPos">指の座標</param>
	/// <returns>当たっていた場合はボタンの定数を返す当たっていなかったら-1</returns>
	int Collision(Vec2 fingPos);
	
private:

	MenuManager& _manager;
};

