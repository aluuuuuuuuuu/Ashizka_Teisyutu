#pragma once
#include "Vec3.h"
#include <memory>
#include "Components.h"
#include "Constant.h"

class ItemBase :
	public Transform,
	public Model,
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemBase();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const = 0;

	/// <summary>
	/// アイテムタイプの取得
	/// </summary>
	/// <returns>アイテムタイプ定数</returns>
	int GetType() const;

	/// <summary>
	/// 自身の削除を始める
	/// </summary>
	void KillItem();
	
	/// <summary>
	/// 自身の死亡フラグを返す
	/// </summary>
	/// <returns>true:死んでいる</returns>
	bool IsDead() const;

	/// <summary>
	/// 自身の無効フラグを返す
	/// </summary>
	/// <returns>true:無効</returns>
	bool IsInvalid() const;

protected:

	// 死亡フラグ
	bool _deadFlag = false;

	// 自身のアイテムの種類
	int _itemType = -1;

	// 無効フラグ
	bool _invalidFlag = false;

	// プレイヤーと当たったフラグ
	bool _playerCollisionFlag = false;

	// 移動ベクトル
	Vec3 _moveVec;

	// 下降が停止してからのフレームカウンタ
	int _frame = 0;
};


