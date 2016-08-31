#include "Item.h"

#include "..//Systems/Scene_System.h"

Item::Item()
{
	InternalTimer = 0;
}


Item::~Item()
{
}

void Item::Update(double dt)
{
	if (Active)
	{
		InternalTimer += (float)dt;
	}
	float MaxWaitTime = Duration + CoolDown;
	if (InternalTimer > MaxWaitTime)
	{
		Active = false;
	}
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

void Item::Use(float dt)
{
	if (!Active)
	{
		float MaxWaitTime = Duration + CoolDown;
		if (InternalTimer < Duration)
		{
			Active = true;
			CoolDown = false;
		}
		else CoolDown = true;
		if (InternalTimer > MaxWaitTime)
		{
			CoolDown = false;
			InternalTimer = 0;
			Active = false;
		}
	}
}