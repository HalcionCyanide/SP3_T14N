#include "BillboardManager.h"
#include <algorithm>

void BillboardManager::UpdateContainer(float dt, const Vector3 &CameraPosition){
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); it++)
	{
		Billboard* B = *it;
		B->Active = B->CheckLife();
		B->PlayerPosition = CameraPosition;
		if (B->Active)
		{
			B->CurrentTime += dt;
			B->Position += B->Velocity * dt;
		}
	}
	std::sort(&BillboardContainer[0], &BillboardContainer[BillboardContainer.size() - 1]);

}

Billboard* BillboardManager::FetchB()
{
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		Billboard* B = *it;
		if (B->Active == false)
		{
			B->Active = true;
			B->CurrentTime = 0;
			return B;
		}
	}
	// No inactive object found
	for (int i = 0; i < 10; i++)
	{
		BillboardContainer.push_back(new Billboard());
	}
	Billboard* B = BillboardContainer.back();
	B->Active = true;
	B->CurrentTime = 0;
	return B;
}

void BillboardManager::AddParticle(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition, const float& LifeTime)
{
	Billboard* B = FetchB();
	B->MeshName = MeshName;
	B->Position = Position;
	B->Dimensions = Dimensions;
	B->Velocity = Velocity;
	B->PlayerPosition = PlayerPosition;
	B->CurrentTime = 0;
	B->LifeTime = LifeTime;
}

void BillboardManager::AddHMapBillboard(const std::string& MeshName, std::vector<unsigned char>& heightMap, const Vector3& TerrainScale, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition)
{
	Billboard* B = FetchB();
	B->MeshName = MeshName;
	B->Position = Position;
	B->Position.y = Dimensions.y * 0.5f + TerrainScale.y * ReadHeightMap(heightMap, Position.x / TerrainScale.x, Position.z / TerrainScale.z);
	B->Dimensions = Dimensions;
	B->Velocity = Velocity;
	B->PlayerPosition = PlayerPosition;
	B->CurrentTime = 0;
	B->LifeTime = -1;
}

void BillboardManager::AddBillboard(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition)
{
	Billboard* B = FetchB();
	B->MeshName = MeshName;
	B->Position = Position;
	B->Dimensions = Dimensions;
	B->Velocity = Velocity;
	B->PlayerPosition = PlayerPosition;
	B->CurrentTime = 0;
	B->LifeTime = -1;
}
