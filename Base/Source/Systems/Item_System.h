#ifndef ITEM_SYSTEM_H
#define ITEM_SYSTEM_H

#include "GenericSystem.h"
#include "..//Classes/Item.h"
#include <map>

class Item_System : public GenericSystem
{
public:
	Item_System();
	virtual ~Item_System();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();

	// Represents the items and whether they are active
	std::map<Item*, bool> ActiveItemMap;

private:
	// Creation Functions
	void Item_System::CreateNewItem(const Item::ItemType& TypeOfItem, const float& Duration = -1, const float& CoolDown = -1, const float& EffectiveValue = 1);

};

#endif