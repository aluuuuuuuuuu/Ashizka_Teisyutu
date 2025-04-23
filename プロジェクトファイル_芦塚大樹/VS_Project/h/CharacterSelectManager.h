#pragma once
#include <memory>
#include "PlayerManager.h"
#include "SceneSelect.h"
#include "Vec2.h"
#include "Constant.h"

class CharacterCard;
class SelectFinger;
class CharacterSelectManager:
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="plData">プレイヤーデータの参照</param>
	CharacterSelectManager(PlayerData& plData);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterSelectManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 開始ボタンが押されたかどうか返す
	/// </summary>
	/// <returns>押されたらture</returns>
	bool GetStart()const;

	/// <summary>
	/// プレイヤーデータを作成する
	/// </summary>
	void CreateData();

	/// <summary>
	/// 戻るフラグを取得する
	/// </summary>
	/// <returns>true:戻る</returns>
	bool GetReturnFlag();

private:

	/// <summary>
	/// 指とカードが重なっているか判定したのち、カードのフラグを変更する
	/// </summary>
	void FingerFunction();
	
	/// <summary>
	/// fingerでAボタンが押されたときにカードの上にあるか判定する関数
	/// </summary>
	/// <param name="fing">判定するfinger</param>
	/// <param name="card">判定するcard</param>
	/// <returns>カードの上にあればtrue</returns>
	bool IsFingerOnCard(std::shared_ptr<SelectFinger>& fing, std::shared_ptr<CharacterCard>& card);

	// 指ポインタ
	std::shared_ptr<SelectFinger>_pFinger[4];

	// カードポインタ
	std::shared_ptr<CharacterCard> _pCard[4];

	// プレイヤーデータの参照
	PlayerData& _plData;

	// 全指が選択し終わったフラグ
	bool _selectFinishFlag;

	// スタートボタンが押されたフラグ
	bool _pushStart;

	// 人数選択に戻るフラグ
	bool _returnFlag;

	// スタートボタン画像ハンドル
	int _startButtonHandle;

	// スタートボタン座標
	Vec2 _startButtonPos;

	bool _viewStartButtonFlag;
};

