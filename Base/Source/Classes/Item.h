/****************************************************************************/
/*!
\file Item.h
\author Tan Teck Ling
\par email: 
\brief
Defines a container that represents an item in game
*/
/****************************************************************************/

#ifndef ITEM_H
#define ITEM_H

#include "GenericEntity.h"

class Item: public GenericEntity
{
public:
	enum ItemType
	{
		IT_INSTANT_HEAL = 0,
		IT_HEAL_OVER_TIME,
		IT_BOOST_SPEED,
		IT_BOOST_INERTIA,
		IT_ITEMTYPE_NUM
	};

	Item();
	virtual ~Item();

	// Update function to handle timers/cooldown
	virtual void Update(double dt);

	// Setters
	virtual void SetItemType(const Item::ItemType &type);
	virtual void SetActive(const bool &active);
	virtual void SetCoolingDown(const bool &coolingdown);
	virtual void SetDuration(const float &duration);
	virtual void SetCoolDown(const float &cooldown);
	virtual void SetEffectiveValue(const float &value);

	// Getters
	Item::ItemType GetItemType()const;
	bool GetActive()const;
	bool GetCoolingDown()const;
	float GetDuration()const;
	float GetCoolDown()const;
	float GetEffectiveValue()const;

	// Use call, resets Timer if valid
	void Use(float dt);

private:
	ItemType TypeOfItem;	//Type of item
	bool Active;			//If item is currently in use
	bool CoolingDown;		//If item is in cooldown state
	float Duration;			//Duration of the item
	float CoolDown;			//Time before Item can be used again(After the effect is over)
	float EffectiveValue;	//Value change
	float InternalTimer;
};

#endif