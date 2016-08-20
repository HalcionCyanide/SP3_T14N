#include "BillboardManager.h"
#include <algorithm>

BillboardManager::BillboardManager()
{
	Init();
}

BillboardManager::~BillboardManager()
{
	Exit();
}

void BillboardManager::Init()
{

}

void BillboardManager::Update(double dt)
{
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Update((float)dt);
	}
}

void BillboardManager::Render()
{
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Render();
	}
}

void BillboardManager::Exit()
{
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		(*it)->Exit();
		delete *it;
	}
	BillboardContainer.clear();
}

void BillboardManager::UpdateContainer(float dt, const Vector3 &CameraPosition){
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); it++)
	{
		Billboard* B = *it;
		B->SetPlayerPosition(CameraPosition);
	}
	Update(dt);
	if (BillboardContainer.size() > 0)
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
			B->SetCurrentTime(0);
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
	B->SetCurrentTime(0);
	return B;
}

void BillboardManager::AddParticle(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition, const float& LifeTime)
{
	Billboard* B = FetchB();
	B->SetParameters(Position, Dimensions, PlayerPosition, Velocity, MeshName, 0, LifeTime);
}

void BillboardManager::AddHMapBillboard(const std::string& MeshName, std::vector<unsigned char>& heightMap, const Vector3& TerrainScale, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition)
{
	Billboard* B = FetchB();
	Vector3 Pos = Position;
	Pos.y = Dimensions.y * 0.5f + TerrainScale.y * ReadHeightMap(heightMap, Position.x / TerrainScale.x, Position.z / TerrainScale.z);
	B->SetParameters(Pos, Dimensions, PlayerPosition, Velocity, MeshName, 0, -1);

}

void BillboardManager::AddBillboard(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition)
{
	Billboard* B = FetchB();
	B->SetParameters(Position, Dimensions, PlayerPosition, Velocity, MeshName, 0, -1);

}