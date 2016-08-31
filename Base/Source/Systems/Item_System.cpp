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
	// Default Item Params
	CreateNewItem(Item::IT_INSTANT_HEAL, -1.f, 3.f, 20);
	CreateNewItem(Item::IT_HEAL_OVER_TIME, 10.f, 5.f, 5);
	CreateNewItem(Item::IT_BOOST_SPEED, 10.f, 7.f, 25);
	CreateNewItem(Item::IT_BOOST_INERTIA, 10.f, 7.f, 50);
}

void Item_System::Update(double dt)
{
	// Loop to update the manager's container
	for (std::map<Item*, bool>::iterator it = ActiveItemMap.begin(); it != ActiveItemMap.end(); ++it)
	{
		// Item is in use
		if ((*it).second)
		{
			if (!(*it).first->GetActive())
				(*it).first->Use((float)dt);
			// Update items if they are active
			(*it).first->Update((float)dt);
		}
	}
}

void Item_System::Exit()
{
	if (ActiveItemMap.empty() == false)
	{
		for (auto it : ActiveItemMap)
			delete &(it.first);		
		ActiveItemMap.clear();
	}
}

void Item_System::CreateNewItem(const Item::ItemType& TypeOfItem, const float& Duration, const float& CoolDown, const float& EffectiveValue)
{
	Item* NewI = new Item();
	NewI->SetActive(false);
	NewI->SetCoolingDown(false);
	NewI->SetItemType(TypeOfItem);
	NewI->SetCoolDown(CoolDown);
	NewI->SetDuration(Duration);
	NewI->SetEffectiveValue(EffectiveValue);
	
	ActiveItemMap.insert(std::pair<Item*, bool>(NewI, false));
}
