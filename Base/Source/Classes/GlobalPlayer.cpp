#include "GlobalPlayer.h"

// Constructor
GlobalPlayer::GlobalPlayer()
{
	Init(0, 1, 1, false);
}

GlobalPlayer::GlobalPlayer(const int& Spell_Power, const int& CurrentHealth, const int& MaxHealth, const bool& IsInteracting)
{
	Init(Spell_Power, CurrentHealth, MaxHealth, IsInteracting);
}

// Destructor
GlobalPlayer::~GlobalPlayer()
{
	Exit();
}

// Main
void GlobalPlayer::Init(const int& Spell_Power, const int& CurrentHealth, const int& MaxHealth, const bool& IsInteracting)
{
	this->Spell_Power = Spell_Power;
	this->CurrentHealth = CurrentHealth;
	this->MaxHealth = MaxHealth;
	this->IsInteracting = IsInteracting;
}

void GlobalPlayer::Update(float dt)
{
	// DO STUFF
}

void GlobalPlayer::Exit()
{
	// SAVE STATS

	// CLEAN UP

}

// Getters
int GlobalPlayer::GetSpellPower()
{
	return Spell_Power;
}

int GlobalPlayer::GetCurrentHealth()
{
	return CurrentHealth;
}

int GlobalPlayer::GetMaxHealth()
{
	return MaxHealth;
}

bool GlobalPlayer::GetIsInteracting()
{
	return IsInteracting;
}

// Setters
void GlobalPlayer::SetSpellPower(const int& Spell_Power)
{
	this->Spell_Power = Spell_Power;
}

void GlobalPlayer::SetCurrentHealth(const int& CurrentHealth)
{
	this->CurrentHealth = CurrentHealth;
}

void GlobalPlayer::SetMaxHealth(const int& MaxHealth)
{
	this->MaxHealth = MaxHealth;
}

void GlobalPlayer::SetIsInteracting(const bool& IsInteracting)
{
	this->IsInteracting = IsInteracting;
}