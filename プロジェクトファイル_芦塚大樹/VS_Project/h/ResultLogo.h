#pragma once

class ResultLogo
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultLogo();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ResultLogo();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
private:

	// ロゴ画像ハンドル
	int _logoHandle;
};

