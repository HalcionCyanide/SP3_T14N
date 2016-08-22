#include "BattleSystem.h"
#include "Scene_System.h"
#include <string>
#include "../Classes/Boundary2D.h"

// To Do List
// Enemy Class [params: atk rate, atk type, atk damage]
// Projectile Class [params: accel, mass, inherit baseobj]
// Upgrade UI_Sys
// Coll Check Sys

BattleSystem::BattleSystem()
{
	//Init();
}

BattleSystem::~BattleSystem()
{
	Exit();
}


// Public Function Calls
void BattleSystem::Init()
{
	CursorPosition = Scene_System::accessing().cSS_InputManager->GetMousePosition();

	// Note Init Player and Enemy Here
	CenterPosition.Set(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	PlayerScale = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.03f;
	PlayerObj = new BaseObject("TFB_Gem", 3, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), Vector3(0, 0, 0), 0, Vector3(0, 0, 1));

	// Background If Needed
	BaseExterior = new BaseObject("GBox", 0.f, CenterPosition + Vector3(0, CenterPosition.y * 0.1f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.6f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.85f, 1), Vector3(), 0, Vector3(0, 0, 1));
	BaseInterior = new BaseObject("GBox", 0.f, CenterPosition + Vector3(0, CenterPosition.y * 0.1f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.7f, 1), Vector3(), 0, Vector3(0, 0, 1));
	
	CurrentEnemy = new Enemy();
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
	//BaseExterior->Render();
	BaseInterior->Render();
	PlayerObj->Render(); // <- Don't bother checking if player is true because if player is not true, something is wrong, thus a crash should be efficient in telling one that. c:
	for (std::vector<BaseObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		if ((*it)->Active && (*it)->Visible)
		{
			(*it)->Render();
		}
	}
}

void BattleSystem::Exit()
{
	if (PlayerObj)
	{
		delete PlayerObj;
		PlayerObj = nullptr;
	}
	if (BaseExterior != nullptr)
	{
		delete BaseExterior;
		BaseExterior = nullptr;
	}
	if (BaseInterior != nullptr)
	{
		delete BaseInterior;
		BaseInterior = nullptr;
	}
	for (std::vector<BaseObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		(*it)->Exit();
		delete *it;
	}
	cBS_ObjectContainer.clear();
}

// Private Function Calls
// State Calls
void BattleSystem::UpdatePlayerTurn(float dt)
{
	// UI Pops In from bottom
	// Mouse Toggle On if WASD mode
}

void BattleSystem::UpdateInventoryScreen(float dt)
{
	// Text Buttons of Usable Items
}

void BattleSystem::UpdatePESPhase(float dt)
{
	Boundary2D TempBounds;
	TempBounds.CalculateValues(BaseExterior->GetPosition(), BaseExterior->GetDimensions());
	for (std::vector<BaseObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			if (TempBounds.CheckCollision((*it)->GetPosition(), Vector3()))
				(*it)->Update(dt);
			else (*it)->Active = false;
		}
	}
	CurrentEnemy->CurrentTime += dt;
	if (CurrentEnemy->CurrentEnemyWave < CurrentEnemy->MaxEnemyWave)
	{
		if (CurrentEnemy->CurrentAttackCount < CurrentEnemy->AttacksPerWave)
		{
			if (CurrentEnemy->CurrentTime > CurrentEnemy->AttackSpeed)
			{
				CurrentEnemy->CurrentTime = 0;
				CurrentEnemy->CurrentAttackCount++;
				BatchCreateAttacks(0);
			}
		}
		else
		{
			CurrentEnemy->CurrentAttackCount = 0;
			CurrentEnemy->CurrentEnemyWave++;
		}
	}
	UpdatePlayer(dt);
	UpdatePhysics(dt);
}

void BattleSystem::UpdateSealPhase(float dt)
{
	// Monster Lerp to Center, GB derender, Seal Appears, Monster Shrink and go into seal, blah blah
}

void BattleSystem::UpdateFleePhase(float dt)
{
	// Survive 1 turn of PES at double damage
	// Escape Screen
}

// Player Calls
void BattleSystem::UpdatePlayer(float dt)
{
	UpdateControls(dt);
	UpdateITimer(dt);
	PlayerObj->Update(dt);
}

void BattleSystem::UpdateITimer(float dt)
{
	if (isInvincible && IFrameTimer > Math::EPSILON)
	{
		IFrameTimer -= dt;
		if (isInvincible && IFrameTimer < Math::EPSILON)
		{
			isInvincible = false;
			IFrameTimer = 0.f;
		}
	}
}

void BattleSystem::UpdateControls(float dt)
{
	Boundary2D TempBounds;
	TempBounds.CalculateValues(BaseInterior->GetPosition(), BaseInterior->GetDimensions());
	if (TempBounds.CheckCollision(Scene_System::accessing().cSS_InputManager->GetMousePosition(), Vector3()))
	{
		CursorPosition = Scene_System::accessing().cSS_InputManager->GetMousePosition();
	}
	float ForceIncrement = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.7f;
	switch (MouseModeSelected)
	{
	case (true) :
	{
		Vector3 PlayerDirection = (CursorPosition - PlayerObj->GetPosition());
		if (!PlayerDirection.IsZero())
			if (PlayerDirection.LengthSquared() < (Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.2f))
				PlayerDirection.SetZero();
			else PlayerDirection.Normalize();
			PlayerActingForce.Set(ForceIncrement * PlayerDirection.x, ForceIncrement * PlayerDirection.y);
			break;
	}
	case (false) :
	{
		if (Scene_System::accessing().cSS_InputManager->GetKeyValue('W'))
		{
			PlayerActingForce.y = ForceIncrement;
		}
		if (Scene_System::accessing().cSS_InputManager->GetKeyValue('S'))
		{
			PlayerActingForce.y = -ForceIncrement;
		}
		if (!Scene_System::accessing().cSS_InputManager->GetKeyValue('W') && !Scene_System::accessing().cSS_InputManager->GetKeyValue('S'))
		{
			PlayerActingForce.y = 0;
		}
		if (Scene_System::accessing().cSS_InputManager->GetKeyValue('A'))
		{
			PlayerActingForce.x = -ForceIncrement;
		}
		if (Scene_System::accessing().cSS_InputManager->GetKeyValue('D'))
		{
			PlayerActingForce.x = ForceIncrement;
		}
		if (!Scene_System::accessing().cSS_InputManager->GetKeyValue('A') && !Scene_System::accessing().cSS_InputManager->GetKeyValue('D'))
		{
			PlayerActingForce.x = 0;
		}
		break;
	}
	}
	if (PlayerObj->GetMass() > Math::EPSILON)
	{
		// Velocity Due To Acceleration If Mass Exists
		PlayerObj->SetVelocity(PlayerObj->GetVelocity() + PlayerActingForce * (1.f / PlayerObj->GetMass()) * dt);
	}
	PlayerObj->SetVelocity(PlayerObj->GetVelocity() - PlayerObj->GetVelocity()*FrictionDecrementMultiplier * dt);
	Vector3 FwdPos = PlayerObj->GetPosition() + PlayerObj->GetVelocity() * dt;
	if (abs(FwdPos.x - TempBounds.GetPosition().x) > TempBounds.GetScale().x * 0.5f - PlayerObj->GetDimensions().x * 0.5f)
	{
		if (abs(PlayerObj->GetVelocity().x * 0.5f) < Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.01f)
			PlayerObj->SetVelocity(Vector3(0, PlayerObj->GetVelocity().y, 0.f));
		else PlayerObj->SetVelocity(Vector3(-PlayerObj->GetVelocity().x * 0.5f, PlayerObj->GetVelocity().y, 0.f));
	}		
	if (abs(FwdPos.y - TempBounds.GetPosition().y) > TempBounds.GetScale().y * 0.5f - PlayerObj->GetDimensions().y * 0.5f)
	{
		if (abs(PlayerObj->GetVelocity().y * 0.5f) < Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.01f)
			PlayerObj->SetVelocity(Vector3(PlayerObj->GetVelocity().x, 0, 0.f));
		else PlayerObj->SetVelocity(Vector3(PlayerObj->GetVelocity().x, -PlayerObj->GetVelocity().y * 0.5f, 0.f));
	}
}

// Physics Related
void BattleSystem::UpdatePhysics(float dt)
{
	if (!isInvincible && cBS_ObjectContainer.size() > 0)
	{
		for (std::vector<BaseObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
		{
			if (CollisionCheck(*PlayerObj, **it, dt))
			{
				// HP Decrement Should Be In CRes
				CollisionResponse(*PlayerObj, **it, dt);
			}
		}
	}
}

bool BattleSystem::CollisionCheck(const BaseObject& BaseObject1, const BaseObject& BaseObject2, float dt)
{
	// Consider Making a projectile class that holds coltype as well as bounds
	return false;
}

bool BattleSystem::CollisionResponse(const BaseObject& BaseObject1, const BaseObject& BaseObject2, float dt)
{
	// Do stuff like hp decrement
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
	Boundary2D TempBounds;
	TempBounds.CalculateValues(BaseExterior->GetPosition(), BaseExterior->GetDimensions());
	Boundary2D TempBounds2;
	TempBounds2.CalculateValues(BaseInterior->GetPosition(), BaseInterior->GetDimensions());
	float XSpawn = TempBounds.GetScale().x * 0.5f - TempBounds2.GetScale().x * 0.5f;
	float YSpawn = TempBounds.GetScale().x * 0.5f - TempBounds2.GetScale().x * 0.5f;
	Vector3 SpawnPos = CenterPosition + Vector3(Math::RandFloatMinMax(-CenterPosition.x - XSpawn, CenterPosition.x + XSpawn), Math::RandFloatMinMax(-CenterPosition.y - YSpawn, CenterPosition.y + YSpawn));
	if (TempBounds.CheckCollision(SpawnPos, Vector3()) && !TempBounds2.CheckCollision(SpawnPos, Vector3()))
	{
		Vector3 DVec = (PlayerObj->GetPosition() - SpawnPos).Normalize() * 100; // * Projectile Speed;
		// Use a fetch Func
		BaseObject* NewObj = new BaseObject("ayylmao", 3, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), DVec, 0, Vector3(0, 0, 1));
		cBS_ObjectContainer.push_back(NewObj);
	}
	else Attack_Bullet();
}
