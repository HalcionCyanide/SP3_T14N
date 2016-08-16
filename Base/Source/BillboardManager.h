#ifndef BILLBOARDMANAGER_H
#define BILLBOARDMANAGER_H

#include "Billboard.h"
#include <vector>

class BillboardManager{
public:
	std::vector<Billboard*> BillboardContainer;

	BillboardManager(){ FetchB(); };

	void UpdateContainer(float dt, Vector3 CameraPosition){
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
	Billboard* FetchB()
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

	void AddBillboard(Vector3 Position, Vector3 Dimensions, Vector3 Velocity, Vector3 PlayerPosition, float LifeTime)
	{
		Billboard* B = FetchB();
		B->Position = Position;
		B->Dimensions = Dimensions;
		B->Velocity = Velocity;
		B->PlayerPosition = PlayerPosition;
		B->CurrentTime = 0;
		B->LifeTime = LifeTime;
	}
};

#endif