#ifndef BILLBOARDMANAGER_H
#define BILLBOARDMANAGER_H

#include "Billboard.h"
#include <vector>
#include "LoadHmap.h"
#include "GenericSystem.h"

class BillboardManager : public GenericSystem
{
public:
	BillboardManager();
	virtual ~BillboardManager();

	std::vector<Billboard*> BillboardContainer;

	void UpdateContainer(float dt, const Vector3 &CameraPosition);

	void AddParticle(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition, const float& LifeTime = 0);

	void AddHMapBillboard(const std::string& MeshName, std::vector<unsigned char>& heightMap, const Vector3& TerrainScale, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity = 0, const Vector3& PlayerPosition = 0);

	void AddBillboard(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity = 0, const Vector3& PlayerPosition = 0);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Billboard* FetchB();
};

#endif