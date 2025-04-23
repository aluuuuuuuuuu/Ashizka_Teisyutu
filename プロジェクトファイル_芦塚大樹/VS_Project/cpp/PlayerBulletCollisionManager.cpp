#include "PlayerBulletCollisionManager.h"
#include "BulletManager.h"
#include "PlayerManager.h"
#include "Player.h"

PlayerBulletCollisionManager::PlayerBulletCollisionManager(std::shared_ptr<BulletManager>& bull, std::shared_ptr<PlayerManager>& pl) :
	_pBulletManager(bull),
	_pPlayerManager(pl)
{
}

PlayerBulletCollisionManager::~PlayerBulletCollisionManager()
{
}

void PlayerBulletCollisionManager::Update()
{
	// プレイヤー分ループを回す
	for (auto& player : _pPlayerManager->GetPlayerList()) {
		// バレット分ループを回す
		for (auto& bullet : _pBulletManager->GetBulletList()) {

			if (bullet->GetPlayerCollisionFlag()) continue;

			// 判定を行う
			if (capsuleSphereCollision(player->_capsuleData.FrontPointA, player->_capsuleData.FrontPointB, player->_capsuleData.Radius, bullet->Position, bullet->GetRadius())) {

				// 当たったことをプレイヤーに伝える
				player->BulletCollision(bullet->GetBulletType());
				
				// 当たったことをバレットに伝える
				bullet->PlayerCollision();
			}
		}
	}
}

Vec3 PlayerBulletCollisionManager::closestPointOnSegment( Vec3& A, Vec3& B, Vec3& P)
{
	Vec3 AB = B - A;
	Vec3 AP = P - A;
	float t = AP.dot(AB) / AB.SqLength();
	t = fmax(0.0f, fmin(1.0f, t)); // クランプ
	return A + AB * t;
}

bool PlayerBulletCollisionManager::capsuleSphereCollision(Vec3& A, Vec3& B, float capsuleRadius, Vec3& sphereCenter, float sphereRadius)
{
	Vec3 closestPoint = closestPointOnSegment(A, B, sphereCenter);
	float distanceSquared = (sphereCenter - closestPoint).SqLength();
	float radiusSum = capsuleRadius + sphereRadius;
	return distanceSquared <= (radiusSum * radiusSum);
}