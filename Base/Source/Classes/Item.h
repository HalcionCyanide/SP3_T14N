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
	virtual void SetDuration(const float &duration);
	virtual void SetEffectiveValue(const float &value);

	// Getters
	Item::ItemType GetItemType()const;
	bool GetActive()const;
	float GetDuration()const;
	float GetEffectiveValue()const;

	// Use call, resets Timer if valid
	void Use(float dt);

	float InternalTimer;

private:
	ItemType TypeOfItem;	//Type of item
	bool Active;			//If item is currently in use
	float Duration;			//Duration of the item
	float EffectiveValue;	//Value change
};

#endif