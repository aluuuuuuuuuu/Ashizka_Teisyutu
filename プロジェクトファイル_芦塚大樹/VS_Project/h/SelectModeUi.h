#pragma once
#include "Vec2.h"

constexpr int SOLO_MODE = 0;
constexpr int MULTI_MODE = 1;
constexpr int TUTORIAL_MODE = 2;
constexpr int OPTION_MODE = 3;

constexpr float SOLO_ANGLE = 315.0f;
constexpr float MULTI_ANGLE = 45.0f;
constexpr float TUTORIAL_ANGLE = 225.0f;
constexpr float OPTION_ANGLE = 135.0f;

constexpr int PLAYER_NUM_1 = 0;
constexpr int PLAYER_NUM_2 = 1;
constexpr int PLAYER_NUM_3 = 2;
constexpr int PLAYER_NUM_4 = 3;

constexpr int MARGIN_X = 90;
constexpr int MARGIN_Y = 90;

class SelectModeUi
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="selectMode">true:セレクトモード</param>
	SelectModeUi(bool selectMode);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectModeUi();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 選択しているボタンを取得する
	/// </summary>
	/// <returns>選択しているボタン</returns>
	int GetSelect() const;
private:

	// フレームカウンタ
	int _frame;

	// 選択している項目の番号
	int _selectButtonNum;

	// 中央の円のハンドル
	int _circleHandle[4];

	// 中央のロゴハンドル
	int _circleLogoHandle;

	// 中央の矢印
	int _centerArrowHandle[4];

	// 選択している方向に白い余白を表示する画像ハンドル
	int _marginHandle[4];

	// 中央の円の座標
	Vec2 _circlePos;

	// 円の拡大率
	double _circleScale;

	// 円の角度
	double _circleAngle;

	// 各ボタンハンドル
	int _buttonHandle[4];

	// フレームハンドル
	int _frameHandle[4];

	// ボタンの座標
	Vec2 _buttonPos[4];

	// 選択しているボタンを目立たせる画像ハンドル	
	int _highlightHandle[4];

	// 下のメッセージハンドル
	int _messageHandle[4];

	// 選択を切り替えたfrag
	bool _changeFrag;

	// 前フレームで選択していたボタン
	int _oldSelectButtonNum;

	// 選んだ項目によって全体の画像の位置をよらせる
	Vec2 _marginPos;

	bool _input;

	// 矢印の移動ベクトル
	Vec2 _arrowVec;

	// 円の拡大率
	float _scale = 0.0f;
};

