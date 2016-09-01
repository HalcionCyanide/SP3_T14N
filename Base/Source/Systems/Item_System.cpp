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
	CreateNewItem(Item::IT_BOOST_INERTIA, 5.f, 5.f, 50);
}

void Item_System::Update(double dt)
{
	// Loop to update the manager's container
	for (std::map<Item*, bool>::iterator it = ActiveItemMap.begin(); it != ActiveItemMap.end(); ++it)
	{
		// Item is in use
		if ((*it).second)
		{
			(*it).first->Use((float)dt);
		}
		(*it).first->Update((float)dt);
		if (!(*it).first->GetActive())
			(*it).second = false; 
	}
}

void Item_System::Exit()
{
	if (ActiveItemMap.empty() == false)
	{
		for (auto it : ActiveItemMap)
			delete (it.first);		
		ActiveItemMap.clear();
	}
}

void Item_System::CreateNewItem(const Item::ItemType& TypeOfItem, const float& Duration, const float& CoolDown, const float& EffectiveValue)
{
	Item* NewI = new Item();
	NewI->SetActive(false);
	NewI->SetItemType(TypeOfItem);
	NewI->SetDuration(Duration);
	NewI->SetEffectiveValue(EffectiveValue);
	
	if (TypeOfItem == Item::IT_INSTANT_HEAL)
	{
		NewI->setName("Heal");
	}
	else if (TypeOfItem == Item::IT_BOOST_INERTIA)
	{
		NewI->setName("Speed");
	}

	ActiveItemMap.insert(std::pair<Item*, bool>(NewI, false));
}
