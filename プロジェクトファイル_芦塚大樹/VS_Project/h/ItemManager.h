#pragma once
#include <memory>
#include <vector>
#include "Vec3.h"
#include "Constant.h"

namespace {
	constexpr int ITEM_TYPE_SPEED = 0;
}

class StageManager;
class ItemBase;
class PlayerManager;
class ItemManager:
	public Constant
{
public:

	
	ItemManager(std::shared_ptr<PlayerManager>& playerManager,std::shared_ptr<StageManager>& stageManager);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ItemManager();

	void Update();

	void Draw() const;


private:

	/// <summary>
	/// プレイヤーとアイテムの当たり判定をとる
	/// </summary>
	void Collision();

	Vec3 ClosestPointLine(Vec3 itemPos, Vec3 capsuleA, Vec3 capsuleB);

	float DistanceClosestPoint(Vec3 closest, Vec3 itemPos);


	std::vector<std::shared_ptr<ItemBase>> _pItem;

	// フレームカウンタ
	int _frame;

	std::shared_ptr<PlayerManager>& _pPlayer;

	std::shared_ptr<StageManager>& _pStage;
};

