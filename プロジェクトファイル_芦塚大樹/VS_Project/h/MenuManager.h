#pragma once
#include <memory>
#include "Vec2.h"
#include "Constant.h"

#define BATU_BUTTON 0
#define MANUAL_BUTTON 1
#define SE_BUTTON 2
#define BGM_BUTTON 3
#define END_BUTTON 4
#define ALL_BUTTON_NUM 5

#define ZERO_GRAPH 0
#define MIN_GRAPH 1
#define MIDDLE_GRAPH 2
#define MAX_GRAPH 3

class MenuCollisionManager;
class MenuButton;
class SelectFinger;
class MenuManager:
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="padNum">操作するパッドナンバー</param>
	MenuManager(int padNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MenuManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 座標を取得する
	/// </summary>
	/// <param name="buttonNum">ボタンの種類番号</param>
	/// <returns>座標</returns>
	Vec2 GetPos(int buttonNum);

	/// <summary>
	/// 当たり判定範囲を取得する
	/// </summary>
	/// <param name="buttonNum">ボタンの種類番号</param>
	/// <returns>レンジ</returns>
	Vec2 GetRange(int buttonNum);

private:

	// 関数ポインタ
	using _updateFunc_t = void(MenuManager::*)();
	using _drawFunc_t = void (MenuManager::*)() const;

	// アップデート関数、ドロー関数
	_updateFunc_t _updateFunc = nullptr;
	_drawFunc_t _drawFunc = nullptr;

	void NormalUpdate();

	void NormalDraw() const;

	void SlideInUpdate();

	void SlideOutUpdate();

	void FadeOutUpdate();

	void FadeOutDraw() const;

	void EndUpdate();

	void EndDraw() const;

	// メニューの基盤画像
	int _menuBordHandle;

	// ぼかし画像ハンドル
	int _backHandle;

	// 基盤の表示座標
	Vec2 _bordPos;

	// 選択指のポインタ
	std::shared_ptr<SelectFinger> _pFinger;

	// 選択ボタン
	std::shared_ptr<MenuButton> _pButton[5];

	// ボタンコリジョンマネージャーポインタ
	std::shared_ptr<MenuCollisionManager> _pCollisionManager;

	// スライドイン用の余白
	int _margin;

	// フェード用のフレームカウンタ
	int _frame;

	// 音量調節画像ハンドル配列
	int _volumeSEGraph[4];
	int _volumeBGMGraph[4];
};

