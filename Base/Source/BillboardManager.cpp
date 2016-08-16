#include "BillboardManager.h"
#include <algorithm>

BillboardManager::BillboardManager()
{ 
	FetchB(); 
};

void BillboardManager::UpdateContainer(float dt, Vector3 CameraPosition){
	for (std::vector<Billboard*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); it++)
	{
		Billboard* B = *it;
		B->Active = B->CheckLife();
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

void BillboardManager::AddBillboard(Vector3 Position, Vector3 Dimensions, Vector3 Velocity, Vector3 PlayerPosition, float LifeTime)
{
	Billboard* B = FetchB();
	B->Position = Position;
	B->Dimensions = Dimensions;
	B->Velocity = Velocity;
	B->PlayerPosition = PlayerPosition;
	B->CurrentTime = 0;
	B->LifeTime = LifeTime;
}
