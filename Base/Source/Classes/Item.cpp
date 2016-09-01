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
	if (InternalTimer > Duration)
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

void Item::SetDuration(const float &duration)
{
	this->Duration = duration;
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

float Item::GetDuration()const
{
	return this->Duration;
}

float Item::GetEffectiveValue()const
{
	return this->EffectiveValue;
}

void Item::Use(float dt)
{
	if (!Active)
	{
		float MaxWaitTime = Duration;
		if (InternalTimer < Duration)
		{
			Active = true;
		}
		if (InternalTimer > Duration)
		{
			InternalTimer = 0;
			Active = false;
		}
	}
}