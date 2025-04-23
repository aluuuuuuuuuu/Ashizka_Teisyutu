#pragma once
#include "ItemBase.h"

class ItemManager;
class StageManager;
class ItemSpeedUp :
	public ItemBase
{
public:

	ItemSpeedUp(Vec3 pos, std::shared_ptr<StageManager>& stageManager, ItemManager& itemManager);

	~ItemSpeedUp();

	void Update();

	void Draw() const;

private:
	std::shared_ptr<StageManager>& _pStage;
	ItemManager& _pItem;
};

