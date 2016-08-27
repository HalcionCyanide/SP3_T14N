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
	virtual void Update(float dt);
	virtual void Exit();

	std::map<Item*, int> ItemMap;
};

#endif