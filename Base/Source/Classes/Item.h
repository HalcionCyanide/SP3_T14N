#ifndef ITEM_H
#define ITEM_H

#include "GenericEntity.h"
#include "..//Systems/Scene_System.h"

class Item: public GenericEntity
{
public:
	enum ItemType
	{
		INSTANT_HEAL = 0,
		HEAL_OVER_TIME,
		BOOST_SPEED,
		BOOST_INERTIA,
		ITEMTYPE_NUM
	};

	Item();
	virtual ~Item();

	virtual void SetItemType(const Item::ItemType &type);
	virtual void SetNumberOfItem(const int &number);
	virtual void SetActive(const bool &active);
	virtual void SetTimer(const float &time);
	virtual void SetEffectiveValue(const float &value);

	Item::ItemType GetItemType()const;
	int GetNumberOfItem()const;
	bool GetActive()const;
	float GetDuration()const;
	float GetEffectiveValue()const;

	virtual void Use(float dt);
	virtual void Consumed(float dt);

private:
	ItemType TypeOfItem;	//Type of item
	int Number_of_Item;		//Amount of item
	bool Active;			//If item is currently in use
	float Duration;			//Duration of the item
	float EffectiveValue;	//Value change
};

#endif