#pragma once
#include "Vec2.h"
#include <memory>

class SceneTitle;
class Logo
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Logo(SceneTitle& scene);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Logo();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

private:
	
	// 表示位置
	Vec2 _dispPos;

	// ロゴ画像
	int _logo;
};

