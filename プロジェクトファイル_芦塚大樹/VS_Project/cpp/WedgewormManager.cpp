#include "WedgewormManager.h"
#include "Wedgeworm.h"

WedgewormManager::WedgewormManager()
{
	// モデルのロード
	int modelHandle = MV1LoadModel("data/model/Wedgeworm.mv1");

	// wormオブジェクトの初期化
	int num = 0;
	for (auto& obj : _worm) {
		obj = std::make_shared<Wedgeworm>(num, modelHandle);
		num++;
	}

	MV1DeleteModel(modelHandle);
}

WedgewormManager::~WedgewormManager()
{
}

void WedgewormManager::Update()
{
	// wormの更新処理
	for (auto& obj : _worm) {
		obj->Update();
	}
}

void WedgewormManager::Draw() const
{
	// wormの描画処理
	for (auto& obj : _worm) {
		obj->Draw();
	}
}

Vec3 WedgewormManager::GetPos(int num) const
{
	return _worm[num]->Position;
}
