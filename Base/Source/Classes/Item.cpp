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

void Item::SetCoolingDown(const bool &coolingdown)
{
	this->CoolingDown = coolingdown;
}

void Item::SetTimer(const float &time)
{
	this->Timer = time;
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

int Item::GetNumberOfItem()const
{
	return this->Number_of_Item;
}

bool Item::GetActive()const
{
	return this->Active;
}

bool Item::GetCoolingDown()const
{
	return this->CoolingDown;
}

float Item::GetTimer()const
{
	return this->Timer;
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

void Item::TimeCheck()
{
	if (Active)
	{
		if (Timer >= Duration)
		{
			Active = false;
			Timer = 0;
			CoolingDown = true;
		}
	}
	else if (CoolingDown)
	{
		if (Timer >= CoolDown)
		{
			CoolDown = false;
		}
	}

}

void Item::Use(float dt)
{
	if (Active)
	{
		Consumed(dt);
		TimeCheck();
	}
	else
	{
		if (!CoolingDown)
		{
			if (Number_of_Item > 0)
			{
				--Number_of_Item;
				Timer = 0.f;
				Active = true;
			}
		}
		else
		{
			Timer += dt;
			TimeCheck();
		}
	}
}
		

void Item::Consumed(float dt)
{
	switch (TypeOfItem)
	{
	case INSTANT_HEAL:
	case HEAL_OVER_TIME:
		Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetCurrentHealth() + EffectiveValue * dt);
		Timer += dt;
		break;
	case BOOST_SPEED:
		//Scene_System::accessing().gPlayer->PlayerObj->
		break;
	case BOOST_INERTIA:
		break;
	}
}