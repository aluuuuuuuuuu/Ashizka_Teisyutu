#pragma once
#include "Vec2.h"
#include <memory>
#include "Constant.h"

class SelectFinger;
class CharacterCard:
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="charNum">キャラクターナンバー</param>
	CharacterCard(int charNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterCard();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(std::shared_ptr<SelectFinger>& fing);
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 自身の選択フラグを立てる
	/// </summary>
	/// <param name="plNum">どのプレイヤーに選択されたか</param>
	void SelectCharacter(int plNum);

	/// <summary>
	/// 自身の選択フラグを下げる
	/// </summary>
	/// <param name="plNum">どのプレイヤーに選択解除されたか</param>
	void RejectCharacter(int plNum);

	bool GetSelectFlag();

	bool GetSelectFlag(int plNum);

	/// <summary>
	/// 表示座標を取得する
	/// </summary>
	/// <returns>座標</returns>
	Vec2 GetPos() const;


	/// <summary>
	/// 自身のキャラクターナンバーを返す
	/// </summary>
	/// <returns>キャラクターナンバー</returns>
	int GetCharacterNum()const;

private:

	// カード画像ハンドル
	int _cardHandle;

	// 選択後のイメージ画像ハンドル
	int _cardSelectHandle[4];

	// どのプレイヤーに選ばれたかフラグ
	bool _selectFlag[4];

	// カード描画座標
	Vec2 _pos;

	// 自身のキャラクターナンバー
	int _characterNum;
};

