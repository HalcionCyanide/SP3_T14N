/****************************************************************************/
/*!
\file BattleSystem.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the battle system within the 2D battle screen
*/
/****************************************************************************/

#ifndef _BATTLESYSTEM_H
#define _BATTLESYSTEM_H

#include "../Classes/BattleScreenObject.h"
#include "../Classes/Enemy.h"
#include "GenericSystem.h"
#include "Vector3.h"
#include <vector>
#include "../Classes/Enemy.h"
#include "../Classes/Boundary2D.h"
#include "../Systems/BillboardManager.h"
#include "..\\Systems\\UI_System.h"

// Version 2, Complete Overhaul - Ryan

class BattleSystem : public GenericSystem
{
public:
	enum BS_State
	{
		BS_Null,
		BS_IntroScreen,
		BS_BattlePhase,
		BS_EndScreenSuccess,
		BS_EndScreenFail,
	};

	BattleSystem();
	virtual ~BattleSystem();

	//Public Variables
	BS_State BattleState = BS_IntroScreen;

	// Public Function Calls
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void SetEnemy(Enemy&);

	BillboardManager cBillboardManager;
	UI_System cUI_System;
	float PlayerScale;

private:
	// Private Variables
	Vector3 CursorPosition;
	Vector3 CenterPosition;
	int ActiveBSOCount;
	float SpellPowerRatio, EnemySpellPowerRatio;

	BattleScreenObject* GetInactiveBSO();

	// Private Function Calls and their variables
	void QuickInit(); // Init call to init variables required for a battle screen restart
	void QuickExit(); // Exit call to delete variables unrequired for a battle screen restart

	// State Calls
	// Intro State Call
	void UpdateIntroScreen(float dt);					// Player Box slides from bottom to top, enemy box from top to bottom, both meet at screenheight/2 and stop for a while before continuing out of the screen. 
														// Start rendering the battle screen when the boxes fill up the screen.
	// Intro Calls
	void UpdatePlayerInfoBox(float dt);					// Set Player Info
	void UpdateEnemyInfoBox(float dt);					// Set Enemy Info
	void UpdateInfoBoxAnimation(float dt);				// Update Boxes
	void RenderIntroScreen();
	// Intro Variables
	bool AnimationPaused = false;						// Value for whether they are paused.
	bool AnimationResumed = false;
	float AnimationPauseTimer;					// Timer to check how long they have been stopped.
	float AnimationPauseTime; 						// How long the boxes stay at the screen's center before moving
	bool HasPlayerAndEnemyInfoInit = false;				// Is the Player and Enemy's data inited yet?
	bool IsInfoBoxOut = false;							// Have the boxes gone out of screen?
	UI_Layer* PlayerInfoBox;
	UI_Layer* EnemyInfoBox;
	UI_Layer* EnemyLayer; // Enemy Image
	Vector3 EnemyDefaultPosition;
	// Intro End
	
	// Battle State Call
	void UpdateBattlePhase(float dt);
	// Battle Calls
	void UpdatePlayer(float dt);
	void UpdateITimer(float dt);
	void UpdateControls(float dt);
	void UpdateEnemyLogic(float dt);
	void RenderBattleScreen();
	// Battle Physics Calls
	void UpdatePhysics(float dt);
	bool CollisionCheck(const BattleScreenObject&, const BattleScreenObject&, float dt);
	bool CollisionResponse(BattleScreenObject&, BattleScreenObject&, float dt);
	// Battle Variables
	UI_Layer* BattleBox;
	UI_Element* HealthBarGreen;
	UI_Element* EnemyStaminaBar;
	float GBarPosition;
	float RBarPosition;
	float HealthBarDefaultScale;
	// Player Variables
	float PlayerBaseMovementSpeed;
	float PlayerCurrentMovementSpeed;
	UI_Layer* PlayerInventoryUI;
	bool PlayerIsInvincible = false;
	float PlayerIFrameTimer;
	float FrictionDecrementMultiplier;
	BattleScreenObject* PlayerObj;
	// Enemy Variables
	Enemy* CurrentEnemy;		// To Store Attack Patterns and Stats
	EnemyProjectile CurrentProjectile;
	float EnemyStaminaTimer;	// Default to 30s round
	float CurrentStaminaTimer;
	float EnemyReselectionInterval;
	// Enemy Calls
	int BatchCreateAttacks(EnemyProjectile& CurrentProjectile);
	void AnimateEnemy();
	// Attack Calls
	void Attack_Bullet(EnemyProjectile& CurrentProjectile);
	void Attack_Trap(EnemyProjectile& CurrentProjectile);
	void Attack_HorizontalRay(EnemyProjectile& CurrentProjectile);
	void Attack_VerticalRay(EnemyProjectile& CurrentProjectile);

	// Battle End

	// End State Calls
	void UpdateEndScreenSuccess(float dt);
	void UpdateEndScreenFail(float dt);
	void RenderEndScreenSuccess();
	void RenderEndScreenFail();
	bool SealAnimationOver = false;
	bool UISwapped = false;
	bool RemoveBattleUI = false;
	void ShiftBattleUI();
	// Init Calls
	void InitSuccessScreen();
	void InitFailScreen();
	// End State Variables
	UI_Layer* EndScreenSuccess;
	UI_Layer* EndScreenFail;
	UI_Element* ExitButton;
	bool PlayerWon;
	int SpellPowerIncrement;
	// End State End

	bool RoundOver = false;

	// Base Object Container [The container that holds the enemy projectiles/attacks]
	std::vector<BattleScreenObject*> cBS_ObjectContainer;

};

#endif // _BATTLESYSTEM_H