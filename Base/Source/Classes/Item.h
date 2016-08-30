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
	virtual void SetActive(const bool &active);
	virtual void SetCoolingDown(const bool &coolingdown);
	virtual void SetDuration(const float &duration);
	virtual void SetCoolDown(const float &cooldown);
	virtual void SetEffectiveValue(const float &value);

	Item::ItemType GetItemType()const;
	bool GetActive()const;
	bool GetCoolingDown()const;
	float GetDuration()const;
	float GetCoolDown()const;
	float GetEffectiveValue()const;

	virtual float Use(float dt, const float &);

private:
	ItemType TypeOfItem;	//Type of item
	bool Active;			//If item is currently in use
	bool CoolingDown;		//If item is in cooldown state
	float Duration;			//Duration of the item
	float CoolDown;			//Time before Item can be used again(After the effect is over)
	float EffectiveValue;	//Value change
};

#endif