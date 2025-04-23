#pragma once
#include <memory>
#include "Vec3.h"

class StageManager;
class MapBulletCollisionManager
{
public:
	MapBulletCollisionManager(std::shared_ptr<StageManager>& stage);
	~MapBulletCollisionManager();

	bool CollisionBullet(Vec3 pos, float radius, int bullet);

	bool CollisionSphire(Vec3 max, Vec3 min, Vec3 pos, float radius);



	float DistancePointToBox(Vec3 max, Vec3 min, Vec3 pos, float radius);

	Vec3 ClosestPointBox(Vec3 max, Vec3 min, Vec3 pos);

private:

	std::shared_ptr<StageManager>& _stage;
};

