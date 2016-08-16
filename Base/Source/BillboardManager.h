#ifndef BILLBOARDMANAGER_H
#define BILLBOARDMANAGER_H

#include "Billboard.h"
#include <vector>

class BillboardManager{
public:
	std::vector<Billboard*> BillboardContainer;

	BillboardManager();

	void UpdateContainer(float dt, Vector3 CameraPosition);

	void AddBillboard(Vector3 Position, Vector3 Dimensions, Vector3 Velocity, Vector3 PlayerPosition, float LifeTime = -1);

private:
	Billboard* FetchB();
};

#endif