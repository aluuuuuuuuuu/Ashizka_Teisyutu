#include "ItemManager.h"
#include "ItemBase.h"
#include "ItemSpeedUp.h"
#include "Vec3.h"
#include "PlayerManager.h"
#include "Player.h"
#include <ctime>
#include "StageManager.h"
#include "Application.h"

ItemManager::ItemManager(std::shared_ptr<PlayerManager>& playerManager, std::shared_ptr<StageManager>& stageManager) :
	_frame(0),
	_pPlayer(playerManager),
	_pStage(stageManager)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/ItemManager.csv");

	srand(static_cast<unsigned int>(time(nullptr)));
}

ItemManager::~ItemManager()
{
}

void ItemManager::Update()
{
	// 全アイテムの更新処理」
	for (auto& item : _pItem) {
		item->Update();
	}

	// 死んでいるアイテムがあれば削除
	for (auto it = _pItem.begin(); it != _pItem.end();) {
		if ((*it)->IsDead()) {
			it = _pItem.erase(it);
		}
		else {
			it++;
		}
	}

	// プレイヤーとアイテムの当たり判定をとる
	Collision();

	// フレームカウンタの加算
	_frame++;

	// ５秒に一回ランダムな場所にアイテムを落とす
	if (_frame % Application::GetInstance().GetConstantInt("FRAME_NUM") == 0) {

		int min = GetConstantInt("RANDAM_MIN");
		int max = GetConstantInt("RANDAM_MAX");

		int randX = min + (rand() % (max - min + 1));
		int randZ = min + (rand() % (max - min + 1));

		_pItem.push_back(std::make_shared<ItemSpeedUp>(Vec3{ randX,GetConstantInt("SPAWN_Y"),randZ }, _pStage, *this));
	}
}

void ItemManager::Draw() const
{
	for (auto& item : _pItem) {
		item->Draw();
	}
}

void ItemManager::Collision()
{
	for (auto& pl : _pPlayer->GetPlayerList()) {
		if (pl->GetDeadFlag()) continue;
		for (auto& item : _pItem) {

			if (item->IsInvalid()) continue;

			// 球の座標と線分の線分上の最近接点を求める
			Vec3 closest = ClosestPointLine(item->Position, pl->_capsuleData.PointA, pl->_capsuleData.PointB);

			// 最近接点から球の座標への距離を求める
			float dist = DistanceClosestPoint(closest, item->Position);

			// 距離がお互いの半径の合計より小さければ当たっている
			if (dist <= 4.0f + pl->_capsuleData.Radius) {

				// プレイヤーのアイテム取得の関数を呼ぶ
				pl->GiveItem(ITEM_TYPE_SPEED);

				// アイテムを無効にする関数を呼ぶ
				item->KillItem();
			}
		}
	}
}

Vec3 ItemManager::ClosestPointLine(Vec3 itemPos, Vec3 capsuleA, Vec3 capsuleB)
{
	Vec3 AB = capsuleB - capsuleA;
	Vec3 AP = itemPos - capsuleA;

	float abLenSquared = AB.dot(AB);
	if (abLenSquared == 0.0f) {
		// AとBが同じ点だった場合
		return capsuleA;
	}

	// スカラーtは、線分AB上の最近接点をパラメータ表示したときの係数
	float t = AP.dot(AB) / abLenSquared;

	// tを0〜1にクランプ（線分の範囲を超えないように）
	t = max(0.0f, min(1.0f, t));

	// 最近接点を計算
	return capsuleA + AB * t;
}

float ItemManager::DistanceClosestPoint(Vec3 closest, Vec3 itemPos)
{
	return (itemPos - closest).Length();
}
