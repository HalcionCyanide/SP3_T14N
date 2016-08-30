#include "Item.h"

Item::Item()
{
}


Item::~Item()
{
}


void Item::SetItemType(const Item::ItemType &type)
{
	this->TypeOfItem = type;
}

void Item::SetActive(const bool &active)
{
	this->Active = active;
}

void Item::SetCoolingDown(const bool &coolingdown)
{
	this->CoolingDown = coolingdown;
}

void Item::SetDuration(const float &duration)
{
	this->Duration = duration;
}

void Item::SetCoolDown(const float &cooldown)
{
	this->CoolDown = cooldown;
}

void Item::SetEffectiveValue(const float &value)
{
	this->EffectiveValue = value;
}

Item::ItemType Item::GetItemType()const
{
	return this->TypeOfItem;
}

bool Item::GetActive()const
{
	return this->Active;
}

bool Item::GetCoolingDown()const
{
	return this->CoolingDown;
}

float Item::GetDuration()const
{
	return this->Duration;
}

float Item::GetCoolDown()const
{
	return this->CoolDown;
}

float Item::GetEffectiveValue()const
{
	return this->EffectiveValue;
}

float Item::Use(float dt, const float &value)
{
	float finalValue;
	switch (TypeOfItem)
	{
	case HEAL_OVER_TIME:
		finalValue = EffectiveValue * dt;
		break;
	case BOOST_INERTIA:
	case INSTANT_HEAL:
	case BOOST_SPEED:
		finalValue = EffectiveValue;
		break;
	}
	return finalValue;
}