#include "Item_System.h"


Item_System::Item_System()
{
	Init();
}

Item_System::~Item_System()
{
	Exit();
}

void Item_System::Init()
{
}

void Item_System::Update(float dt)
{

}

void Item_System::Exit()
{
	if (ItemMap.empty() == false)
	{
		for (auto it : ItemMap)
			delete &(it.second);		
		ItemMap.clear();
	}

}