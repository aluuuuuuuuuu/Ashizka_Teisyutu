#include "ItemSpeedUp.h"
#include "DxLib.h"
#include "ItemManager.h"
#include "StageManager.h"
#include "Application.h"

ItemSpeedUp::ItemSpeedUp(Vec3 pos, std::shared_ptr<StageManager>& stageManager, ItemManager& itemManager):
	_pStage(stageManager),
	_pItem(itemManager)
{
	// 初期位置の設定
	Position = pos;
	Scale = _pItem.GetConstantFloat("SPEED_UP_SCALE");

	_moveVec.y = _pItem.GetConstantFloat("SPEED_UP_SPEED");

	// アイテムタイプの設定
	_itemType = ITEM_TYPE_SPEED;

	// モデルの設定
	InitModel(MV1LoadModel("data/model/pumpkin.mv1"));	
}

ItemSpeedUp::~ItemSpeedUp()
{
}

void ItemSpeedUp::Update()
{
	auto& app = Application::GetInstance();

	if (_invalidFlag) {
		_frame++;
		if (_frame > _pItem.GetConstantInt("LIFE_TIME")) {
			_deadFlag = true;
		}
	}
	else {
		Position += _moveVec;

		int x = static_cast<int>( Position.x / _pStage->GetConstantInt("BLOCK_SIZE")) + 1;
		int z = static_cast<int>( Position.z / _pStage->GetConstantInt("BLOCK_SIZE")) + 1;

		if (_pStage->GetStageInfo(x, 1, z) != 0) {
			if (Position.y < _pItem.GetConstantFloat("STOP_POS_2")) {
				Position.y = _pItem.GetConstantFloat("STOP_POS_2");
			}

			if (Position.y == _pItem.GetConstantFloat("STOP_POS_2")) {
				_frame++;

				if (_frame > app.GetConstantInt("FRAME_NUM") * 3) {
					_invalidFlag = true;
					_frame = 0;
				}
			}
		}
		else if (_pStage->GetStageInfo(x, 0, z) != 0) {
			if (Position.y < _pItem.GetConstantFloat("STOP_POS_1")) {
				Position.y = _pItem.GetConstantFloat("STOP_POS_1");
			}

			if (Position.y == _pItem.GetConstantFloat("STOP_POS_1")) {
				_frame++;

				if (_frame > app.GetConstantInt("FRAME_NUM") * 3) {
					_invalidFlag = true;
					_frame = 0;
				}
			}
		}
		else {
			if (Position.y < _pItem.GetConstantInt("DEAD_LINE")) {
				_deadFlag = true;
			}
		}
	}
	UpdateModel(GetTransformInstance());
}

void ItemSpeedUp::Draw() const
{
	DrawModel();
}
