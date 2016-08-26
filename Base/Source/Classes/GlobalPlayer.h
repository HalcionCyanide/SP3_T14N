/****************************************************************************/
/*!
\file SceneBattleScreen.cpp
\author Ryan Lim Rui An, Lee Sek Heng
\par email: 150577L@mymail.nyp.edu.sg, 150629Z@mymail.nyp.edu.sg
\brief
Defines the global player
*/
/****************************************************************************/

#ifndef _GLOBAL_PLAYER_H
#define _GLOBAL_PLAYER_H

#include "PlayerObject.h"
#include "Camera3.h"
#include <map>

class PlayerObject;
class Camera3;

class GlobalPlayer : public GenericEntity
{
public:
	// Constructor
	GlobalPlayer();
	GlobalPlayer(const int& Spell_Power, const int& CurrentHealth, const int& MaxHealth, const bool& IsInteracting);

	// Destructor
	virtual ~GlobalPlayer();

	// Main
	void Init(const int& Spell_Power, const int& CurrentHealth, const int& MaxHealth, const bool& IsInteracting);
	void Update(float dt);
	void Exit();

	PlayerObject* PlayerObj;

	// Getters
	int GetSpellPower();
	int GetCurrentHealth();
	int GetMaxHealth();
	bool GetIsInteracting();

	// Setters
	void SetSpellPower(const int&);
	void SetCurrentHealth(const int&);
	void SetMaxHealth(const int&);
	void SetIsInteracting(const bool&);

    bool LoadPlayerSave(const std::string &fileName);
    bool RewritePlayerSave(const std::string &fileName);

    void SetCurrCam(Camera3 &theCam);

	Camera3 *CurrCamera;
    std::string currSceneID;

	std::map < std::string, int >  playerCurrQState;
private:
	int Spell_Power;
	int CurrentHealth;
	int MaxHealth;
	bool IsInteracting;

	// Quest Container
	// Inventory Container
	// Money
	// Current Equipment
	// Exp
	// Level
};

#endif // _GLOBAL_PLAYER_H