#pragma once
#include "Vec2.h"

class SceneTitle;
class AnyPushButton
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="scene">タイトルシーンの参照</param>
	AnyPushButton(SceneTitle& scene);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AnyPushButton();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
private:	

	// 文章画像のハンドル
	int _textHandle;

	// 表示位置
	Vec2 _dispPos;

	// フレームカウント
	int _frame;
};

