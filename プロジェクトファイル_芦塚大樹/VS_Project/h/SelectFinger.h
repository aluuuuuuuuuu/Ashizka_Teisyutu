#pragma once
#include "Vec2.h"
#include "Vec3.h"

class SelectFinger
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="padNum">パッドナンバー</param>
	SelectFinger(int padNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SelectFinger();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;
	
	/// <summary>
	/// 今フレームでいずれかのボタンが押されたかどうか
	/// </summary>
	/// <param name="button">ボタン定数</param>
	/// <returns>押されていたらtrue</returns>
	bool IsPushButton(int button) const;

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <returns>二次元座標</returns>
	Vec2 GetPos() const;

	/// <summary>
	/// パッドナンバーを返す
	/// </summary>
	/// <returns>パッドナンバー</returns>
	int GetPadNum() const;

	/// <summary>
	/// 選択フラグを変更する
	/// </summary>
	/// <param name="flag">選択したかどうか</param>
	void SetSelectFlag(bool flag);

	/// <summary>
	/// 選択しているかどうか返す
	/// </summary>
	/// <returns>選択していたらtrue</returns>
	bool IsSelected()const;

	/// <summary>
	/// 選択するキャラをセットする
	/// </summary>
	/// <param name="charaNum">選択するキャラのナンバー</param>
	void SetSelectChara(int charaNum);

	/// <summary>
	/// 選択したキャラナンバーを返す
	/// </summary>
	/// <returns>キャラナンバー</returns>
	int GetSelectChara()const;

private:

	// パッドナンバー
	int _padNum;

	// 最大プレイヤー数分の指画像ハンドル
	int _fingerHandle;

	// 表示座標
	Vec2 _pos;

	// 選択したフラグ
	bool _selectFlag;

	// どのキャラクターを選択したか
	int _chara;
};

