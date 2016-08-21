#ifndef _BATTLESYSTEM_H
#define _BATTLESYSTEM_H

#include "../Classes/BaseObject.h"
#include "GenericSystem.h"
#include "Vector3.h"
#include <vector>

class BattleSystem : public GenericSystem
{
public:
	enum BS_State
	{
		BS_Null,
		BS_PlayerTurn,
		BS_Inventory,
		BS_PlayerEvasionStage,
		BS_Seal,
		BS_Flee,
	};

	BattleSystem();
	virtual ~BattleSystem();

	//Public Variables
	BaseObject* Player;
	BS_State BattleState = BS_PlayerEvasionStage;// = BS_Null;

	// Public Function Calls
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	// Enemy* Enemy <- To Store Attack Patterns and Stats
	// void SetEnemy(const Enemy&)
	// Misc. Enemy Functions

private:
	// Private Variables
	Vector3 CursorPosition;
	float PlayerScale;
	Vector3 PlayerActingForce;
	Vector3 CenterPosition;
	BaseObject* BaseExterior;
	BaseObject* BaseInterior;
	bool MouseModeSelected = true;
	bool isInvincible = false;
	float IFrameTimer = 0;
	const float FrictionDecrementMultiplier = 0.8f;

	// Private Function Calls
	// State Calls
	void UpdatePlayerTurn(float dt);
	void UpdateInventoryScreen(float dt);
	void UpdatePESPhase(float dt);
	void UpdateSealPhase(float dt);
	void UpdateFleePhase(float dt);

	// Player Calls
	void UpdatePlayer(float dt);
	void UpdateITimer(float dt);
	void UpdateControls(float dt);

	// Physics Related
	void UpdatePhysics(float dt);
	bool CollisionCheck(const BaseObject& BaseObject1, const BaseObject& BaseObject2, float dt);
	bool CollisionResponse(const BaseObject& BaseObject1, const BaseObject& BaseObject2, float dt);

	// Enemy Calls [Based on type use a specific attack call]
	int BatchCreateAttacks(const int& AttackType);

	// Attack Calls // Think of better names later
	void Attack_Bullet();

	// Base Object Container [The container that holds the enemy projectiles/attacks]
	std::vector<BaseObject*> cBS_ObjectContainer;

};

#endif // _BATTLESYSTEM_H