#ifndef _BATTLESYSTEM_H
#define _BATTLESYSTEM_H

#include "../Classes/BattleScreenObject.h"
#include "../Classes/Enemy.h"
#include "GenericSystem.h"
#include "Vector3.h"
#include <vector>
#include "../Classes/Enemy.h"
#include "../Classes/Boundary2D.h"

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
		BS_Fail,
	};

	BattleSystem();
	virtual ~BattleSystem();

	//Public Variables
	BattleScreenObject* PlayerObj;
	BS_State BattleState = BS_PlayerEvasionStage;// = BS_Null;

	// Public Function Calls
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	BattleScreenObject* GetInactiveBSO();

	//void SetPlayer();
	void SetEnemy(Enemy&);

private:
	// Private Variables
	Enemy* CurrentEnemy; //<- To Store Attack Patterns and Stats

	Boundary2D InteriorBounds, ExteriorBounds;

	Vector3 CursorPosition;
	float PlayerScale;
	Vector3 CenterPosition;
	BattleScreenObject* BaseExterior;
	BattleScreenObject* BaseInterior;
	bool MouseModeSelected = false;
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
	bool CollisionCheck(const BattleScreenObject&, const BattleScreenObject&, float dt);
	bool CollisionResponse(const BattleScreenObject&, const BattleScreenObject&, float dt);

	// Enemy Calls [Based on type use a specific attack call]
	int BatchCreateAttacks(EnemyProjectile& CurrentProjectile);

	// Attack Calls // Think of better names later
	void Attack_Bullet(EnemyProjectile& CurrentProjectile);
	void Attack_Trap(EnemyProjectile& CurrentProjectile);

	// Base Object Container [The container that holds the enemy projectiles/attacks]
	std::vector<BattleScreenObject*> cBS_ObjectContainer;

};

#endif // _BATTLESYSTEM_H