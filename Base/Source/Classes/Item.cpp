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

void Item::SetNumberOfItem(const int &number)
{
	this->Number_of_Item = number;
}

void Item::SetActive(const bool &active)
{
	this->Active = active;
}

void Item::SetTimer(const float &time)
{
	this->Duration = time;
}

void Item::SetEffectiveValue(const float &value)
{
	this->EffectiveValue = value;
}

Item::ItemType Item::GetItemType()const
{
	return TypeOfItem;
}

int Item::GetNumberOfItem()const
{
	return Number_of_Item;
}

bool Item::GetActive()const
{
	return Active;
}

float Item::GetDuration()const
{
	return Duration;
}

float Item::GetEffectiveValue()const
{
	return EffectiveValue;
}

void Item::Use(float dt)
{
	if (Number_of_Item > 0)
	{
		--Number_of_Item;
		Consumed(dt);
	}
}

void Item::Consumed(float dt)
{
	switch (TypeOfItem)
	{
	case INSTANT_HEAL:
		Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetCurrentHealth() + EffectiveValue);
		break;
	}
}