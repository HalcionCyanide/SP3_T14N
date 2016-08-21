#include "BattleSystem.h"
#include "Scene_System.h"

BattleSystem::BattleSystem()
{
	Init();
}

BattleSystem::~BattleSystem()
{
	Exit();
}


// Public Function Calls
void BattleSystem::Init()
{
	// Note Init Player and Enemy Here
	CenterPosition.Set(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	PlayerScale = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.03f;
	Player = new BaseObject("Player", 3, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), Vector3(0, 0, 0), 0, Vector3(0, 0, 1));

	// Background If Needed

}

void BattleSystem::Update(double dt)
{
	// Update Calls Sorted by State
	switch (BattleState)
	{
	case BS_PlayerTurn:
		UpdatePlayerTurn((float)dt);
		break;
	case BS_Inventory:
		UpdateInventoryScreen((float)dt);
		break;
	case BS_PlayerEvasionStage:
		UpdatePESPhase((float)dt);
		break;
	case BS_Seal:
		UpdateSealPhase((float)dt);
		break;
	case BS_Flee:
		UpdateFleePhase((float)dt);
		break;
	}
}

void BattleSystem::Render()
{
	Player->Render(); // <- Don't bother checking if player is true because if player is not true, something is wrong, thus a crash should be efficient in telling one that. c:
	for (std::vector<BaseObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		(*it)->Render();
	}
}

void BattleSystem::Exit()
{
	if (Player)
		delete Player;
}

// Private Function Calls
// State Calls
void BattleSystem::UpdatePlayerTurn(float dt)
{

}

void BattleSystem::UpdateInventoryScreen(float dt)
{

}

void BattleSystem::UpdatePESPhase(float dt)
{
	for (std::vector<BaseObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		(*it)->Update(dt);
	}
	UpdateITimer(dt);
	UpdateControls(dt);
	UpdatePhysics(dt);
	UpdatePlayer(dt);
}

void BattleSystem::UpdateSealPhase(float dt)
{

}

void BattleSystem::UpdateFleePhase(float dt)
{

}


// Player Calls
void BattleSystem::UpdatePlayer(float dt)
{

}

void BattleSystem::UpdateITimer(float dt)
{

}

void BattleSystem::UpdateControls(float dt)
{

}


// Physics Related
void BattleSystem::UpdatePhysics(float dt)
{
	// Do CCheck & CRes if required
}

bool BattleSystem::CollisionCheck(const BaseObject& BaseObject1, const BaseObject& BaseObject2, float dt)
{
	return false;
}

bool BattleSystem::CollisionResponse(const BaseObject& BaseObject1, const BaseObject& BaseObject2, float dt)
{
	return false;
}


// Enemy Calls [Based on type use a specific attack call]
int BattleSystem::BatchCreateAttacks(const int& AttackType)
{
	switch (AttackType)
	{
	case 0: 
		int AttackCount = Math::RandIntMinMax(1, 5);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_Bullet(); 
		return AttackCount;
		break;
	}
	return 0;
}


// Attack Calls // Think of better names later // Pass Vel Pos Offset?
void BattleSystem::Attack_Bullet()
{
	// Creation Func
}
