#pragma once


class ResultUi
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="time">撃破タイム</param>
	ResultUi(int time);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ResultUi();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
private:

	// リザルトロゴ画像ハンドル
	int _resultLogoHandle;

	// 赤線画像ハンドル
	int _redLineHandle;

	// 撃破タイム文字画像ハンドル
	int _timeStringHandle;

	// ランク枠画像ハンドル
	int _rankFrame;

	// ランク画像ハンドル
	int _rankHandle;

	// ボタン説明画像ハンドル
	int _guide;
};

