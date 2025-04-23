#pragma once

class SelectUI
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectUI();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	~SelectUI();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
	
private:
	// ファイターを選べ文画像ハンドル
	int _selectFighterHandle;

	// 戻る画像
	int _returnHandle;
};

