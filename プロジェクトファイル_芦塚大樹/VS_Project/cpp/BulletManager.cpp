#include "BulletManager.h"
#include "NormalBullet.h"
#include "Vec3.h"
#include "MapBulletCollisionManager.h"
#include "GrapplerBullet.h"
#include "BombBullet.h"

BulletManager::BulletManager(std::shared_ptr<MapBulletCollisionManager>& col, std::shared_ptr<WedgewormManager>& worm) :
	_collManager(col),
	_wedgeManager(worm)
{
	// 定数の読み込み
	ReadCSV("data/constant/BulletManager.csv");
}

BulletManager::~BulletManager()
{
}

void BulletManager::Update()
{
	// バレットの更新
	for (auto& bullet : _pBullet) {
		bullet->Update();
	}

	// バレットが死んでいたら削除する
	for (auto it = _pBullet.begin(); it != _pBullet.end();) {
		if ((*it)->IsDead()) {
			it = _pBullet.erase(it);	//要素を削除し次の要素を指すイテレータを取得
		}
		else {
			it++;
		}
	}
}

void BulletManager::Draw() const
{
	for (auto& bullet : _pBullet) {
		bullet->Draw();
	}
}

void BulletManager::PushBullet(int bul, Vec3 dist, Vec3 pos, int plnum)
{
	switch (bul)
	{
	case NORMAL_BULLET:
		_pBullet.push_back(std::make_shared<NormalBullet>(dist, pos, _collManager, *this, plnum));
		break;
	case GRAPPLER_BULLET:
		_pBullet.push_back(std::make_shared<GrapplerBullet>(dist, pos, _collManager, *this, plnum, _wedgeManager));
		break;
	case BOMB_BULLET:
		_pBullet.push_back(std::make_shared<BombBullet>(dist, pos, _collManager, *this, plnum));
		break;
	default:
		break;
	}
}

bool BulletManager::IsCollisionBullet(int plNum)
{

	for (auto& bullet : _pBullet) {
		if (bullet->GetBulletType() == GRAPPLER_BULLET && bullet->GetPlayerNum() == plNum) {
			if (bullet->GetCollisionFlag()) {
				return true;
			}
		}
	}

	return false;
}

bool BulletManager::GetInvalidFlag(int plNum) const
{
	for (auto& bullet : _pBullet) {
		if (bullet->GetBulletType() == GRAPPLER_BULLET && bullet->GetPlayerNum() == plNum) {
			if (bullet->GetInvalidFlag()) {
				return true;
			}
		}
	}
	return false;
}

void BulletManager::KillBullet(int plNum)
{
	for (auto& bullet : _pBullet) {
		if (bullet->GetBulletType() == GRAPPLER_BULLET && bullet->GetPlayerNum() == plNum) {
			bullet->KillBullet();
		}
	}
}

Vec3 BulletManager::GetBulletPos(int plNum)
{
	for (auto& bullet : _pBullet) {
		if (bullet->GetBulletType() == GRAPPLER_BULLET && bullet->GetPlayerNum() == plNum) {
			return bullet->Position;
		}
	}
	return Vec3();
}

bool BulletManager::GetBulletExist(int plNum)
{
	for (auto& bullet : _pBullet) {
		if (bullet->GetBulletType() == GRAPPLER_BULLET && bullet->GetPlayerNum() == plNum) {
			return true;
		}
	}
	return false;
}

std::list<std::shared_ptr<BulletBase>>& BulletManager::GetBulletList()
{
	return _pBullet;
}

