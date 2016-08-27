#include "BattleSystem.h"
#include "Scene_System.h"
#include "..//Scenes//Scene_MainMenu.h"
#include <string>

BattleSystem::BattleSystem()
{
    CurrentEnemy = nullptr;
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

	PlayerObj = nullptr; // new BattleScreenObject("TFB_Gem", 1, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	CurrentEnemy = nullptr;
	PlayerInventoryUI = nullptr;
	PlayerInfoBox = nullptr;
	EnemyInfoBox = nullptr;
	EnemyLayer = nullptr;
}

void BattleSystem::QuickInit()
{
	// Intro
	AnimationPaused = false;
	AnimationPauseTimer = 0.f;
	AnimationPauseTime = 2.f;
	HasPlayerAndEnemyInfoInit = false;
	IsInfoBoxOut = false;
	BattleState = BS_IntroScreen;
	
	// Player
	PlayerBaseMovementSpeed = PlayerScale;
	PlayerCurrentMovementSpeed = PlayerBaseMovementSpeed;
	PlayerIsInvincible = false;
	PlayerIFrameTimer = 0;
	FrictionDecrementMultiplier = 0.8f;

	// Enemy
	EnemyStaminaTimer = 30; // 30s round

	// BSOs
	PlayerObj = new BattleScreenObject("TFB_Gem", 1, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));

	// UIs
	PlayerInventoryUI = new UI_Layer();
	PlayerInfoBox = new UI_Layer();
	EnemyInfoBox = new UI_Layer();
}

void BattleSystem::Update(double dt)
{
	// Update Calls Sorted by State
	switch (BattleState)
	{
	case BS_IntroScreen:
		UpdateIntroScreen((float)dt);
		break;
	case BS_BattlePhase:
		//UpdateBattlePhase((float)dt);
		break;
	case BS_EndScreenSuccess:
		break;
	case BS_EndScreenFail:
		break;
	}
}

void BattleSystem::Render()
{
	cBillboardManager.Render();
	cUI_System.Render();
	if (PlayerObj)
		PlayerObj->Render(); // <- Don't bother checking if player is true because if player is not true, something is wrong, thus a crash should be efficient in telling one that. c:
	for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
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

	if (PlayerInventoryUI)
	{
		PlayerInventoryUI->Exit();
		delete PlayerInventoryUI;
	}

	if (PlayerInfoBox)
	{
		PlayerInfoBox->Exit();
		delete PlayerInfoBox;
	}

	if (EnemyInfoBox)
	{
		EnemyInfoBox->Exit();
		delete EnemyInfoBox;
	}

	/*if (EnemyLayer)
	{
		EnemyLayer->Exit();
		delete EnemyLayer;
	}*/

	if (CurrentEnemy)
	{
		delete CurrentEnemy;
		CurrentEnemy = nullptr;
	}

	PlayerInfoBox = PlayerInventoryUI = EnemyInfoBox = EnemyLayer = nullptr;

	for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		(*it)->Exit();
		delete *it;
	}
	cBS_ObjectContainer.clear();
}

BattleScreenObject* BattleSystem::GetInactiveBSO()
{
	// Container has stuff
	if (cBS_ObjectContainer.size() > 0)
	{
		for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
			if (!(*it)->Active)
			{
				(*it)->LifeTimer = 0;
				(*it)->LifeTime = -1;
				(*it)->Visible = true;
				return *it;
			}
	}
	// No Active/Container has nothing, make more BSOs
	for (unsigned short i = 0; i < 5; ++i)
	{
		BattleScreenObject* BSO = new BattleScreenObject("", 1, 0, Vector3(1, 1, 1), 0, 0, Vector3(0, 0, 1));
		BSO->Active = false;
		cBS_ObjectContainer.push_back(BSO);
	}
	return cBS_ObjectContainer.back();
}

void BattleSystem::SetEnemy(Enemy& E)
{
	CurrentEnemy = new Enemy(E);
	EnemyLayer = new UI_Layer();
	EnemyLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, CurrentEnemy->MeshName, Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 4.f, 0), Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 4.f, 0), Vector3(400, 400, 1), Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 1.6f, 0));
	cUI_System.cUIS_LayerContainer.push_back(EnemyLayer);
	QuickInit();
}

// Private Function Calls
// State Calls
void BattleSystem::UpdateIntroScreen(float dt)
{
	if (!HasPlayerAndEnemyInfoInit)
	{
		HasPlayerAndEnemyInfoInit = true;
		// Set the data
		UpdatePlayerInfoBox(dt);
		UpdateEnemyInfoBox(dt);
	}
	// Animate the boxes
	UpdateInfoBoxAnimation(dt);
}

void BattleSystem::UpdatePlayerInfoBox(float dt)
{
	// Set Spawn and Target
	Vector3 Target = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.25f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	PlayerInfoBox->LayerOriginalPosition = PlayerInfoBox->LayerCenterPosition = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.25f, -2.f * Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 0);
	PlayerInfoBox->LayerTargetPosition = Target;
	// Layer Backing Image
	PlayerInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "door", Target, Target, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.25f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 1), Target);
}

void BattleSystem::UpdateEnemyInfoBox(float dt)
{
	EnemyInfoBox->LayerOriginalPosition = PlayerInfoBox->LayerCenterPosition = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.75f, 2.f * Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 0);
	EnemyInfoBox->LayerTargetPosition = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.75f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

}

void BattleSystem::UpdateInfoBoxAnimation(float dt)
{

}

// Player Calls
void BattleSystem::UpdatePlayer(float dt)
{
	UpdateControls(dt);
	UpdateITimer(dt);
}

void BattleSystem::UpdateITimer(float dt)
{
	if (PlayerIsInvincible && PlayerIFrameTimer > Math::EPSILON)
	{
		PlayerIFrameTimer -= dt;
		int RPcount = Math::RandIntMinMax(1, 2);
		for (int i = 0; i < RPcount; ++i)
			cBillboardManager.AddParticle("ParticleW", PlayerObj->GetPosition(), Vector3(PlayerScale * 0.75f, PlayerScale * 0.75f, 1), Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale), 0), Vector3(0, 0, 1), 2);

		if (PlayerIsInvincible && PlayerIFrameTimer < Math::EPSILON)
		{
			PlayerIsInvincible = false;
			PlayerIFrameTimer = 0.f;
		}
	}
}

void BattleSystem::UpdateControls(float dt)
{
	
}

// Physics Related
void BattleSystem::UpdatePhysics(float dt)
{
	if (!PlayerIsInvincible && cBS_ObjectContainer.size() > 0)
	{
		for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
		{
			if ((*it)->Active && (*it)->Visible)
				if (CollisionCheck(*PlayerObj, **it, dt))
				{
					// HP Decrement Should Be In CRes
					if (CollisionResponse(*PlayerObj, **it, dt))
					{
						PlayerIsInvincible = true;
						PlayerIFrameTimer = 2;
					}
				}
		}
	}
}

bool BattleSystem::CollisionCheck(const BattleScreenObject& BSO1, const BattleScreenObject& BSO2, float dt)
{
	// Consider Making a projectile class that holds coltype
	switch (BSO2.Type)
	{
		case BattleScreenObject::BS_Normal:
		{

			break;
		}
		case BattleScreenObject::BS_Bullet:
		{
			// Simple Circle BC.
			float CombinedRadiusSquared = (BSO1.GetDimensions().x * 0.4f + BSO2.GetDimensions().x * 0.4f) * (BSO1.GetDimensions().x * 0.4f + BSO2.GetDimensions().x * 0.4f);
			float DistSquared = ((BSO1.GetPosition() + BSO1.GetVelocity() * dt) - (BSO2.GetPosition() + BSO2.GetVelocity() * dt)).LengthSquared();
			Vector3 RelativeVelocity = BSO2.GetVelocity() - BSO1.GetVelocity();
			Vector3 RelativeDisplacement = BSO1.GetPosition() - BSO2.GetPosition();
			if (DistSquared < CombinedRadiusSquared &&
				RelativeVelocity.Dot(RelativeDisplacement) > 0) // Check Angle Between Relative Velocity Vectors in attempt to see if the angle between them is smaller than zero
			{
				return true;
			}
			break;
		}
		case BattleScreenObject::BS_Trap:
		{
			// Simple Circle BC.
			float CombinedRadiusSquared = (BSO1.GetDimensions().x * 0.3f + BSO2.GetDimensions().x * 0.3f) * (BSO1.GetDimensions().x * 0.3f + BSO2.GetDimensions().x * 0.3f);
			float DistSquared = ((BSO1.GetPosition() + BSO1.GetVelocity() * dt) - (BSO2.GetPosition() + BSO2.GetVelocity() * dt)).LengthSquared();
			Vector3 RelativeVelocity = BSO2.GetVelocity() - BSO1.GetVelocity();
			Vector3 RelativeDisplacement = BSO1.GetPosition() - BSO2.GetPosition();
			if (DistSquared < CombinedRadiusSquared &&
				RelativeVelocity.Dot(RelativeDisplacement) > 0) // Check Angle Between Relative Velocity Vectors in attempt to see if the angle between them is smaller than zero
			{
				return true;
			}
			break;
			break;
		}
		case BattleScreenObject::BS_Bouncer:
		{

			break;
		}
	}
	return false;
}

bool BattleSystem::CollisionResponse(const BattleScreenObject& BSO1, const BattleScreenObject& BSO2, float dt)
{
	// Do stuff like hp decrement
	switch (BSO2.Type)
	{
	case BattleScreenObject::BS_Normal:
	{

		return true;
		break;
	}
	case BattleScreenObject::BS_Bullet:
	{
		EnemyProjectile* P = nullptr;
		for (std::vector<EnemyProjectile*>::iterator it = CurrentEnemy->cE_Projectiles.begin(); it != CurrentEnemy->cE_Projectiles.end(); ++it)
		{
			if ((*it)->getName() == BSO2.GetMeshName())
				P = *it;
		}
		if (P != nullptr)
		{
			Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetCurrentHealth() - P->DamagePerAttack);
		}
		return true;
		break;
	}
	case BattleScreenObject::BS_Trap:
	{
		EnemyProjectile* P = nullptr;
		for (std::vector<EnemyProjectile*>::iterator it = CurrentEnemy->cE_Projectiles.begin(); it != CurrentEnemy->cE_Projectiles.end(); ++it)
		{
			if ((*it)->getName() == BSO2.GetMeshName())
				P = *it;
		}
		if (P != nullptr)
		{
			Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetCurrentHealth() - P->DamagePerAttack);
		}
		return true;
		break;
	}
	case BattleScreenObject::BS_Bouncer:
	{

		return true;
		break;
	}
	}
	return false;
}


// Enemy Calls [Based on type use a specific attack call]
int BattleSystem::BatchCreateAttacks(EnemyProjectile& CurrentProjectile)
{
	if (CurrentProjectile.AttackType == "Bullet")
	{
		int AttackCount = Math::RandIntMinMax(1, CurrentProjectile.BatchCreateCount);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_Bullet(CurrentProjectile);
		return AttackCount;
	}
	else if (CurrentProjectile.AttackType == "Bouncer")
	{
		int AttackCount = Math::RandIntMinMax(1, CurrentProjectile.BatchCreateCount);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_Trap(CurrentProjectile);
		return AttackCount;
	}
	else if (CurrentProjectile.AttackType == "Trap")
	{
		int AttackCount = Math::RandIntMinMax(1, CurrentProjectile.BatchCreateCount);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_Trap(CurrentProjectile);
		return AttackCount;
	}
	return 0;
}


// Attack Calls
void BattleSystem::Attack_Bullet(EnemyProjectile& CurrentProjectile)
{
	// Spawn a bullet at the exterior of the GBox, fire it at the player.
	//float XSpawn = ExteriorBounds.GetDimensions().x * 0.5f - InteriorBounds.GetDimensions().x * 0.5f;
	//float YSpawn = ExteriorBounds.GetDimensions().x * 0.5f - InteriorBounds.GetDimensions().x * 0.5f;
	//Vector3 SpawnPos = CenterPosition + Vector3(Math::RandFloatMinMax(-CenterPosition.x - XSpawn, CenterPosition.x + XSpawn), Math::RandFloatMinMax(-CenterPosition.y - YSpawn, CenterPosition.y + YSpawn));
	//if (ExteriorBounds.CheckCollision(SpawnPos) && !InteriorBounds.CheckCollision(SpawnPos))
	//{
	//	Vector3 DVec = (PlayerObj->GetPosition() - SpawnPos).Normalize() * CurrentProjectile.ScalarAcceleration; 
	//	BattleScreenObject* BSO = GetInactiveBSO();
	//	BSO->SetParameters(CurrentProjectile.StoredMesh, 5, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	//	BSO->Type = BattleScreenObject::BS_Bullet;
	//	BSO->Active = true;
	//	BSO->SetAcceleration(DVec);
	//	BSO->SetMass(3.f);
	//}
	//else Attack_Bullet(CurrentProjectile);
}

void BattleSystem::Attack_Trap(EnemyProjectile& CurrentProjectile)
{
	Vector3 SpawnPos = PlayerObj->GetPosition() + Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale), 0);
	float RAngle = Math::RandFloatMinMax(-360.f, 360.f);
	BattleScreenObject* BSO = GetInactiveBSO();
	BSO->SetParameters("CrossMarker", 3, SpawnPos + Vector3(0, 0, -1), Vector3(PlayerScale, PlayerScale, 1), 0, RAngle, Vector3(0, 0, 1));
	BSO->Type = BattleScreenObject::BS_Null;
	BSO->Active = true;
	BSO->LifeTime = 1;
	BSO->SetAcceleration(0);

	BSO = GetInactiveBSO();
	BSO->SetParameters(CurrentProjectile.StoredMesh, 3, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), Vector3(CurrentProjectile.ScalarAcceleration, CurrentProjectile.ScalarAcceleration), RAngle, Vector3(0, 0, 1));
	CurrentProjectile.setName(BSO->GetMeshName());
	BSO->Type = BattleScreenObject::BS_Trap;
	BSO->Visible = false;
	BSO->Active = true;
	BSO->LifeTime = 4;
	BSO->SetMass(3.f);
}