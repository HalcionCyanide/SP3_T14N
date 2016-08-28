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

	PlayerObj = nullptr; 
	CurrentEnemy = nullptr;
	PlayerInventoryUI = nullptr;
	PlayerInfoBox = nullptr;
	EnemyInfoBox = nullptr;
	EnemyLayer = nullptr;
	BattleBox = nullptr;
	HealthBarGreen = nullptr;
	EnemyStaminaBar = nullptr;
}

void BattleSystem::QuickInit()
{
	// Generic
	SpellPowerRatio = EnemySpellPowerRatio = 0;
	RoundOver = false;

	// Intro
	AnimationPaused = false;
	AnimationResumed = false;
	AnimationPauseTimer = 0.f;
	AnimationPauseTime = 5.f;
	HasPlayerAndEnemyInfoInit = false;
	IsInfoBoxOut = false;
	BattleState = BS_IntroScreen;
	
	// Player
	PlayerBaseMovementSpeed = PlayerScale;
	PlayerCurrentMovementSpeed = PlayerBaseMovementSpeed;
	PlayerIsInvincible = false;
	PlayerIFrameTimer = 0;
	FrictionDecrementMultiplier = 0.9f;

	// Enemy
	EnemyStaminaTimer = 15; // 30s round
	CurrentStaminaTimer = 0;
	EnemyReselectionInterval = 0;

	// BSOs
	PlayerObj = new BattleScreenObject("PlayerHeart", 1, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	ActiveBSOCount = 0;

	// UIs
	//PlayerInventoryUI = new UI_Layer();
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
		UpdateBattlePhase((float)dt);
		break;
	case BS_EndScreenSuccess:
		break;
	case BS_EndScreenFail:
		break;
	}
	if (ActiveBSOCount == 0 && RoundOver)
		QuickExit();
}

void BattleSystem::Render()
{
	switch (BattleState)
	{
	case BS_IntroScreen:
		RenderIntroScreen();
		break;
	case BS_BattlePhase:
		RenderBattleScreen();
		break;
	case BS_EndScreenSuccess:
		break;
	case BS_EndScreenFail:
		break;
	}

	cBillboardManager.Render();
	for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		if ((*it)->Active && (*it)->Visible)
		{
			(*it)->Render();
		}
	}
}

void BattleSystem::RenderIntroScreen()
{
	PlayerInfoBox->Render();
	EnemyInfoBox->Render();
	if (AnimationPaused)
	{
		EnemyLayer->Render();
		BattleBox->Render();
	}
	if (AnimationResumed)
		RenderBattleScreen();
}

void BattleSystem::RenderBattleScreen()
{
	if (PlayerObj)
	{
		if (PlayerIsInvincible)
		{
			if ((int)(PlayerIFrameTimer * 10) % 3 == 0)
				PlayerObj->Render();
		}
		else PlayerObj->Render();

	}
	if (EnemyLayer)
		EnemyLayer->Render();
	if (BattleBox)
		BattleBox->Render();
}

void BattleSystem::Exit()
{
	if (PlayerObj)
	{
		delete PlayerObj;
		PlayerObj = nullptr;
	}
	if (CurrentEnemy)
	{
		delete CurrentEnemy;
		CurrentEnemy = nullptr;
	}

	for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		(*it)->Exit();
		delete *it;
	}
	cBS_ObjectContainer.clear();

	cUI_System.Exit();
}

void BattleSystem::QuickExit()
{
	if (PlayerObj)
	{
		delete PlayerObj;
		PlayerObj = nullptr;
	}

	if (CurrentEnemy)
	{
		delete CurrentEnemy;
		CurrentEnemy = nullptr;
	}

	for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		(*it)->Exit();
		delete *it;
	}
	cBS_ObjectContainer.clear();

	cUI_System.Exit();

	Scene_System::accessing().SwitchToPreviousScene();
	Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
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
	QuickInit();

	float BoxHeightUnits = 12;
	HealthBarDefaultScale = PlayerScale * 13;

	GBarPosition = CenterPosition.x * 0.95f;
	RBarPosition = CenterPosition.x * 1.75f;

	float StaminaRatio = (float)(EnemyStaminaTimer - CurrentStaminaTimer) / EnemyStaminaTimer;
	float RBarWidth = StaminaRatio * HealthBarDefaultScale * 0.5f;
	float cRBarPosition = RBarPosition - (HealthBarDefaultScale - RBarWidth) * 0.5f + HealthBarDefaultScale * 0.25f;

	CurrentEnemy = new Enemy(E);
	EnemyLayer = new UI_Layer();
	BattleBox = new UI_Layer();
	EnemyDefaultPosition.Set(CenterPosition.x * 1.75f, CenterPosition.y * 1.7f, 0);

	EnemySpellPowerRatio = (float)(CurrentEnemy->SpellPower / Scene_System::accessing().gPlayer->GetSpellPower());
	SpellPowerRatio = (float)(Scene_System::accessing().gPlayer->GetSpellPower() / CurrentEnemy->SpellPower);

	// Set Wave Time
	EnemyStaminaTimer *= CurrentEnemy->MaxEnemyWave;
	int EnemyAttack = Math::RandIntMinMax(0, CurrentEnemy->cE_Projectiles.size() - 1);
	CurrentProjectile = *CurrentEnemy->cE_Projectiles[EnemyAttack];

	EnemyLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, CurrentEnemy->MeshName, Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 4.f, 0), Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 4.f, 0), Vector3(PlayerScale * 5, PlayerScale * 5, 1), EnemyDefaultPosition);
	EnemyLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "UI_ChatBox", Vector3(CenterPosition.x * 4.f, CenterPosition.y * 1.35f, 0), Vector3(CenterPosition.x * 4.f, CenterPosition.y * 1.35f, 0), Vector3(PlayerScale * 7, PlayerScale * 1.25f, 1), Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 1.35f, 0), CurrentEnemy->EnemyName);
	
	// Spell Power
	std::stringstream ss;
	ss << "Spell Power: " << CurrentEnemy->SpellPower;
	EnemyLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "UI_ChatBox", Vector3(CenterPosition.x * 4.f, CenterPosition.y * 1.2f, 0), Vector3(CenterPosition.x * 4.f, CenterPosition.y * 1.2f, 0), Vector3(PlayerScale * 7, PlayerScale * 1.25f, 1), Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 1.2f, 0), ss.str());
	
	EnemyLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "UI_ChatBox", Vector3(CenterPosition.x * 2.f, CenterPosition.y * 1.f, 0), Vector3(CenterPosition.x * 4.f, CenterPosition.y * 1.f, 0), Vector3(PlayerScale * 7, PlayerScale * 1.25f, 1), Vector3(CenterPosition.x * 1.75f, CenterPosition.y * 1.f, 0), "Enemy Stamina");
	EnemyStaminaBar = new UI_Element(UI_Element::UI_BUTTON_R_TO_SCRN, "UI_HP_Red", Vector3(CenterPosition.x * 2.f, CenterPosition.y * 0.85f, 0), Vector3(CenterPosition.x * 4.f, CenterPosition.y * 0.85f, 0), Vector3(RBarWidth, PlayerScale * 1.25f, 1), Vector3(cRBarPosition, CenterPosition.y * 0.85f, 0));
	EnemyLayer->cUI_Layer.push_back(EnemyStaminaBar);
	
	cUI_System.cUIS_LayerContainer.push_back(EnemyLayer);

	BattleBox->AddUIElement(UI_Element::UI_LOGO, "GBox", Vector3(CenterPosition.x * -2.f, CenterPosition.y, 0), Vector3(CenterPosition.x * -2.f, CenterPosition.y, 0), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.6f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.6f, 1), Vector3(CenterPosition.x * 0.9f, CenterPosition.y, 0));
	BattleBox->cUI_Layer[0]->UI_Bounds->ResetValues();

	BattleBox->AddUIElement(UI_Element::UI_BUTTON_R_TO_SCRN, "UI_ChatBox", Vector3(CenterPosition.x * -4.f, CenterPosition.y * 0.3f, 0), Vector3(CenterPosition.x * -4.f, CenterPosition.y - PlayerScale *  (1 + BoxHeightUnits * 0.5f), 0), Vector3(PlayerScale * 6, PlayerScale * 1.25f, 1), Vector3(CenterPosition.x * 0.65f, CenterPosition.y * 0.3f, 0), "Remaining Health: ");
	
	float HealthRatio = (float)Scene_System::accessing().gPlayer->GetCurrentHealth() / (float)Scene_System::accessing().gPlayer->GetMaxHealth();
	float GBarWidth = HealthRatio * HealthBarDefaultScale * 0.5f;
	float cGBarPosition = GBarPosition - (HealthBarDefaultScale - GBarWidth) * 0.5f;

	HealthBarGreen = new UI_Element(UI_Element::UI_BUTTON_R_TO_SCRN, "UI_HP_Green", Vector3(CenterPosition.x * 8.f, CenterPosition.y * 0.3f, 0), Vector3(CenterPosition.x * 8.f, CenterPosition.y * 0.3f, 0), Vector3(GBarWidth, PlayerScale * 1.f, 1), Vector3(cGBarPosition, CenterPosition.y * 0.3f, 0));
	BattleBox->cUI_Layer.push_back(HealthBarGreen);

	cUI_System.cUIS_LayerContainer.push_back(BattleBox);
	PlayerObj->SetPosition(BattleBox->cUI_Layer[0]->TargetPosition);
}

// Private Function Calls
// State Update Calls
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
	if (AnimationResumed)
	{
		BattleBox->Update(dt);
		EnemyLayer->Update(dt);
	}
}

void BattleSystem::UpdatePlayerInfoBox(float dt)
{
	// Set Spawn and Target
	Vector3 Target = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.25f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	Vector3 InfoBoxSpawnPos = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * -2.f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	PlayerInfoBox->LayerOriginalPosition.x = PlayerInfoBox->LayerCenterPosition.x = InfoBoxSpawnPos.x;
	PlayerInfoBox->LayerTargetPosition = 0;

	float AspectRatio = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight;

	// Layer Backing Image
	PlayerInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "BS_LPanel", Target, Target, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 1), Target);
	
	// Player Name
	PlayerInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "UI_ChatBox", Target + Vector3(0, Target.y * 0.75f), Target + Vector3(0, Target.y * 0.75f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.4f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.05f * AspectRatio, 1), Target + Vector3(0, Target.y * 0.75f), "You");

	// Player Image
	PlayerInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, PlayerObj->GetMeshName(), Target + Vector3(0, Target.y * 0.2f), Target + Vector3(0, Target.y * 0.2f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.2f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.2f * AspectRatio, 1), Target + Vector3(0, Target.y * 0.2f));

	// Spell Power
	std::stringstream ss;
	ss << "Spell Power: " << Scene_System::accessing().gPlayer->GetSpellPower();
	PlayerInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "UI_ChatBox", Target - Vector3(0, Target.y * 0.3f), Target - Vector3(0, Target.y * 0.3f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.4f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.05f * AspectRatio, 1), Target - Vector3(0, Target.y * 0.3f), ss.str());

	// Health
	ss.str("");
	Scene_System::accessing().gPlayer->GetCurrentHealth();
	ss << "Health: " << Scene_System::accessing().gPlayer->GetCurrentHealth() << " / " << Scene_System::accessing().gPlayer->GetMaxHealth();
	PlayerInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "UI_ChatBox", Target - Vector3(0, Target.y * 0.6f), Target - Vector3(0, Target.y * 0.6f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.4f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.05f * AspectRatio, 1), Target - Vector3(0, Target.y * 0.6f), ss.str());

	cUI_System.cUIS_LayerContainer.push_back(PlayerInfoBox);
}

void BattleSystem::UpdateEnemyInfoBox(float dt)
{
	// Set Spawn and Target
	Vector3 Target = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.75f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	Vector3 InfoBoxSpawnPos = Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 2.f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	EnemyInfoBox->LayerOriginalPosition.x = EnemyInfoBox->LayerCenterPosition.x = InfoBoxSpawnPos.x;
	EnemyInfoBox->LayerTargetPosition = 0;

	float AspectRatio = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight;

	// Layer Backing Image
	EnemyInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "BS_LPanel", Target, Target, Vector3(-Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 1), Target);

	// Enemy Name
	EnemyInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "UI_ChatBox", Target + Vector3(0, Target.y * 0.75f), Target + Vector3(0, Target.y * 0.75f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.4f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.05f * AspectRatio, 1), Target + Vector3(0, Target.y * 0.75f), CurrentEnemy->EnemyName);

	// Enemy Image
	EnemyInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, CurrentEnemy->MeshName, Target + Vector3(0, Target.y * 0.2f), Target + Vector3(0, Target.y * 0.2f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.2f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.2f * AspectRatio, 1), Target + Vector3(0, Target.y * 0.2f));

	// Spell Power
	std::stringstream ss;
	ss << "Spell Power: " << CurrentEnemy->SpellPower;
	EnemyInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "UI_ChatBox", Target - Vector3(0, Target.y * 0.3f), Target - Vector3(0, Target.y * 0.3f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.4f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.05f * AspectRatio, 1), Target - Vector3(0, Target.y * 0.3f), ss.str());

	// Wave Count
	ss.str("");
	ss << "Survive: " << EnemyStaminaTimer << "s";
	EnemyInfoBox->AddUIElement(UI_Element::UI_BUTTON_B_TO_SCRN, "UI_ChatBox", Target - Vector3(0, Target.y * 0.6f), Target - Vector3(0, Target.y * 0.6f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.4f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.05f * AspectRatio, 1), Target - Vector3(0, Target.y * 0.6f), ss.str());

	cUI_System.cUIS_LayerContainer.push_back(EnemyInfoBox);
}

void BattleSystem::UpdateInfoBoxAnimation(float dt)
{
	if (!AnimationPaused && !AnimationResumed)
	{
		float Check1 = (PlayerInfoBox->LayerCenterPosition - PlayerInfoBox->LayerTargetPosition).LengthSquared();
		float Check2 = (EnemyInfoBox->LayerCenterPosition - EnemyInfoBox->LayerTargetPosition).LengthSquared();
		if (Check1 < 10.f && Check2 < 10.f)
		{
			AnimationPaused = true;
			int ParticleCount = Math::RandIntMinMax(50, 100);
			for (int i = 0; i < ParticleCount; ++i)
			{
				cBillboardManager.AddParticle("Fire", CenterPosition + Vector3(0, Math::RandFloatMinMax(-Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight* 0.5f)), Vector3(PlayerScale* 1.5f, PlayerScale * 1.5f, 1.f), Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale)), Vector3(0, 0, 1), 2);
			}
		}
	}
	else if (AnimationPaused && !AnimationResumed)
	{
		AnimationPauseTimer += dt;
		if (AnimationPauseTime < AnimationPauseTimer)
		{
			AnimationResumed = true;
			AnimationPaused = false;
			PlayerInfoBox->SwapOriginalWithTarget();
			EnemyInfoBox->SwapOriginalWithTarget();
		}
	}
	else if(AnimationResumed)
	{
		float Check1 = (PlayerInfoBox->LayerCenterPosition - PlayerInfoBox->LayerTargetPosition).LengthSquared();
		float Check2 = (EnemyInfoBox->LayerCenterPosition - EnemyInfoBox->LayerTargetPosition).LengthSquared();
		if (Check1 < 500.f && Check2 < 500.f)
		{
			BattleState = BS_BattlePhase;
		}
	}
	PlayerInfoBox->Update((float)dt);
	EnemyInfoBox->Update((float)dt);
}

void BattleSystem::UpdateBattlePhase(float dt)
{
	UpdateEnemyLogic(dt);
	EnemyLayer->Update(dt);
	UpdatePhysics(dt);
	UpdatePlayer(dt);
}

void BattleSystem::UpdateEnemyLogic(float dt)
{
	if ((EnemyLayer->cUI_Layer[0]->Position - EnemyLayer->cUI_Layer[0]->TargetPosition).LengthSquared() < 1.f)
		EnemyLayer->cUI_Layer[0]->TargetPosition = EnemyDefaultPosition;
	CurrentStaminaTimer += dt;
	if (CurrentStaminaTimer < EnemyStaminaTimer)
	{
		float StaminaRatio = (float)(EnemyStaminaTimer - CurrentStaminaTimer) / EnemyStaminaTimer;
		float RBarWidth = StaminaRatio * HealthBarDefaultScale * 0.5f;
		float BarPosition = RBarPosition - (HealthBarDefaultScale - RBarWidth) * 0.5f + HealthBarDefaultScale * 0.25f;
		EnemyStaminaBar->Dimensions.x = RBarWidth;
		if (EnemyStaminaBar->AtTarget)
			EnemyStaminaBar->Position.x = BarPosition;
		else if ((EnemyStaminaBar->Position - EnemyStaminaBar->TargetPosition).LengthSquared() < 1.f)
			EnemyStaminaBar->AtTarget = true;

		if (EnemyReselectionInterval < Math::EPSILON)
		{
			// Use Current Attack
			if (CurrentProjectile.StoredMesh && CurrentEnemy->CurrentAttackCount < CurrentProjectile.AttacksPerWave)
			{
				CurrentEnemy->CurrentTime += dt;
				// Can fire an attack
				if (CurrentEnemy->CurrentTime > CurrentProjectile.AttackSpeed)
				{
					CurrentEnemy->CurrentTime = 0;
					CurrentEnemy->CurrentAttackCount++;
					AnimateEnemy();
					BatchCreateAttacks(CurrentProjectile);
				}
			}
			// Reselect an attack from the pool
			else
			{
				EnemyReselectionInterval += CurrentProjectile.AttacksPerWave * CurrentProjectile.AttackSpeed * CurrentProjectile.BatchCreateCount;
				int EnemyAttack = Math::RandIntMinMax(0, CurrentEnemy->cE_Projectiles.size() - 1);
				CurrentProjectile = *CurrentEnemy->cE_Projectiles[EnemyAttack];
				CurrentEnemy->CurrentAttackCount = 0;
			}
		}
		else EnemyReselectionInterval -= dt;
	}
	else // Out of stamina
	{
		RoundOver = true;
	}

	// Bullet Update
	UI_Element* BBox = BattleBox->cUI_Layer[0];
	ActiveBSOCount = 0;
	for (std::vector<BattleScreenObject*>::iterator it = cBS_ObjectContainer.begin(); it != cBS_ObjectContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			if ((*it)->Type == BattleScreenObject::BS_Bullet && 
				(*it)->GetPosition().y - (*it)->GetDimensions().y * 0.5f > BBox->Position.y - BBox->Dimensions.y * 0.5f)
				{
					(*it)->Update(dt);
					++ActiveBSOCount;
				}
			else if ((*it)->Type == BattleScreenObject::BS_Blast || (*it)->Type == BattleScreenObject::BS_Null &&
				BattleBox->cUI_Layer[0]->UI_Bounds->CheckCollision((*it)->GetPosition()))
				{
					(*it)->Update(dt);
					++ActiveBSOCount;
				}
			else if ((*it)->Type == BattleScreenObject::BS_HRay &&
				(*it)->GetPosition().x + (*it)->GetDimensions().x * 0.5f < Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 1.1f)
			{
				(*it)->Update(dt);
				++ActiveBSOCount;
			}
			else if ((*it)->Type == BattleScreenObject::BS_VRay &&
				(*it)->GetPosition().y - (*it)->GetDimensions().y * 0.5f > BBox->Position.y - BBox->Dimensions.y * 0.5f)
				{
					(*it)->Update(dt);
					++ActiveBSOCount;
				}
			else
			{
				--ActiveBSOCount;
				(*it)->Active = false;
			}
		}
	}
}

void BattleSystem::AnimateEnemy()
{
	EnemyLayer->cUI_Layer[0]->TargetPosition = EnemyDefaultPosition + Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale), 0);
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
			cBillboardManager.AddParticle("WhiteParticle", PlayerObj->GetPosition(), Vector3(PlayerScale * 0.75f, PlayerScale * 0.75f, 1), Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale), 0), Vector3(0, 0, 1), 2);

		if (PlayerIsInvincible && PlayerIFrameTimer < Math::EPSILON)
		{
			PlayerIsInvincible = false;
			PlayerIFrameTimer = 0.f;
		}
	}
}

void BattleSystem::UpdateControls(float dt)
{
	float ForceIncrement = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.2f;// *dt;
	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('W'))
	{
		Vector3 Accel = PlayerObj->GetAcceleration();
		Accel.y = ForceIncrement;
		PlayerObj->SetAcceleration(Accel);
	}
	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('S'))
	{
		Vector3 Accel = PlayerObj->GetAcceleration();
		Accel.y = -ForceIncrement;
		PlayerObj->SetAcceleration(Accel);
	}
	if (!Scene_System::accessing().cSS_InputManager->GetKeyValue('W') && !Scene_System::accessing().cSS_InputManager->GetKeyValue('S'))
	{
		Vector3 Accel = PlayerObj->GetAcceleration();
		Accel.y = 0;
		PlayerObj->SetAcceleration(Accel);
	}
	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('A'))
	{
		Vector3 Accel = PlayerObj->GetAcceleration();
		Accel.x = -ForceIncrement;
		PlayerObj->SetAcceleration(Accel);
	}
	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('D'))
	{
		Vector3 Accel = PlayerObj->GetAcceleration();
		Accel.x = ForceIncrement;
		PlayerObj->SetAcceleration(Accel);
	}
	if (!Scene_System::accessing().cSS_InputManager->GetKeyValue('A') && !Scene_System::accessing().cSS_InputManager->GetKeyValue('D'))
	{
		Vector3 Accel = PlayerObj->GetAcceleration();
		Accel.x = 0;
		PlayerObj->SetAcceleration(Accel);
	}

	// BoundsCheck Here
	Vector3 FwdPos = PlayerObj->GetPosition() + PlayerObj->GetVelocity() * dt;
	Boundary2D InteriorBounds = *BattleBox->cUI_Layer[0]->UI_Bounds;
	InteriorBounds.SetPosition(BattleBox->cUI_Layer[0]->Position);
	InteriorBounds.SetDimensions(BattleBox->cUI_Layer[0]->Dimensions);
	InteriorBounds.ResetValues();

	if ((PlayerObj->GetPosition().x + PlayerObj->GetVelocity().x * dt > InteriorBounds.GetPosition().x + InteriorBounds.GetDimensions().x * 0.5f - PlayerObj->GetDimensions().x * 0.5f && PlayerObj->GetVelocity().x > 0) ||
		(PlayerObj->GetPosition().x + PlayerObj->GetVelocity().x * dt< InteriorBounds.GetPosition().x - InteriorBounds.GetDimensions().x * 0.5f + PlayerObj->GetDimensions().x * 0.5f && PlayerObj->GetVelocity().x < 0))
	{
		PlayerObj->SetPosition(PlayerObj->GetPosition() - PlayerObj->GetVelocity() * 0.5f * dt);
		PlayerObj->SetVelocity(Vector3(-PlayerObj->GetVelocity().x * (1.f - FrictionDecrementMultiplier), PlayerObj->GetVelocity().y, 0.f));
		PlayerObj->SetAcceleration(Vector3(0, PlayerObj->GetAcceleration().y, 0.f));
	}
	if ((PlayerObj->GetPosition().y + PlayerObj->GetVelocity().y * dt > InteriorBounds.GetPosition().y + InteriorBounds.GetDimensions().y * 0.5f - PlayerObj->GetDimensions().y * 0.5f && PlayerObj->GetVelocity().y > 0) ||
		(PlayerObj->GetPosition().y + PlayerObj->GetVelocity().y * dt < InteriorBounds.GetPosition().y - InteriorBounds.GetDimensions().y * 0.5f + PlayerObj->GetDimensions().y * 0.5f && PlayerObj->GetVelocity().y < 0))
	{
		PlayerObj->SetPosition(PlayerObj->GetPosition() - PlayerObj->GetVelocity() * 0.5f * dt);
		PlayerObj->SetVelocity(Vector3(PlayerObj->GetVelocity().x, -PlayerObj->GetVelocity().y * (1.f - FrictionDecrementMultiplier), 0.f));
		PlayerObj->SetAcceleration(Vector3(PlayerObj->GetAcceleration().x, 0, 0.f));
	}

	PlayerObj->Update(dt);

	// One Frame's worth of friction
	PlayerObj->SetVelocity(PlayerObj->GetVelocity() - PlayerObj->GetVelocity() * FrictionDecrementMultiplier * dt);
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
						PlayerIFrameTimer = 1 * Math::Clamp(SpellPowerRatio, 0.8f, 1.f);

						int HP = Scene_System::accessing().gPlayer->GetCurrentHealth();
						HP = Math::Clamp(Scene_System::accessing().gPlayer->GetCurrentHealth(), 0, Scene_System::accessing().gPlayer->GetMaxHealth());
						Scene_System::accessing().gPlayer->SetCurrentHealth(HP);

						float HealthRatio = (float)Scene_System::accessing().gPlayer->GetCurrentHealth() / (float)Scene_System::accessing().gPlayer->GetMaxHealth() + 0.001f;
						float GBarWidth = HealthRatio * HealthBarDefaultScale * 0.5f;
						float BarPosition = GBarPosition - (HealthBarDefaultScale - GBarWidth) * 0.5f + HealthBarDefaultScale * 0.25f;
						HealthBarGreen->Dimensions.x = GBarWidth;
                        HealthBarGreen->Position.x = BarPosition;
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
		case BattleScreenObject::BS_Bullet:
		{
			// Simple Circle BC.
			float CollisionRadius = 0.4f;
			float CombinedRadiusSquared = (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius) * (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius);
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
		case BattleScreenObject::BS_HRay:
		{
			// Simple Circle BC.
			float CollisionRadius = 0.4f;
			float CombinedRadiusSquared = (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius) * (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius);
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
		case BattleScreenObject::BS_VRay:
		{
			// Simple Circle BC.
			float CollisionRadius = 0.4f;
			float CombinedRadiusSquared = (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius) * (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius);
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
		case BattleScreenObject::BS_Blast:
		{
			// Simple Circle BC.
			float CollisionRadius = 0.3f;
			float CombinedRadiusSquared = (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius) * (BSO1.GetDimensions().x * CollisionRadius + BSO2.GetDimensions().x * CollisionRadius);
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
	}
	return false;
}

bool BattleSystem::CollisionResponse(const BattleScreenObject& BSO1, const BattleScreenObject& BSO2, float dt)
{
	// Do stuff like hp decrement
	switch (BSO2.Type)
	{
	case BattleScreenObject::BS_Null:
	{
		break;
	}
	default:
	{
		EnemyProjectile* P = nullptr;
		for (std::vector<EnemyProjectile*>::iterator it = CurrentEnemy->cE_Projectiles.begin(); it != CurrentEnemy->cE_Projectiles.end(); ++it)
		{
			if ((*it)->getName() == BSO2.GetMeshName())
				P = *it;
		}
		if (P != nullptr)
		{
			Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetCurrentHealth() - (int(P->DamagePerAttack * Math::Clamp(EnemySpellPowerRatio, 1.f, 1.5f))));
		}
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
	else if (CurrentProjectile.AttackType == "Trap")
	{
		int AttackCount = Math::RandIntMinMax(1, CurrentProjectile.BatchCreateCount);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_Trap(CurrentProjectile);
		return AttackCount;
	}
	else if (CurrentProjectile.AttackType == "VRay")
	{
		int AttackCount = Math::RandIntMinMax(1, CurrentProjectile.BatchCreateCount);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_VerticalRay(CurrentProjectile);
		return AttackCount;
	}
	else if (CurrentProjectile.AttackType == "HRay")
	{
		int AttackCount = Math::RandIntMinMax(1, CurrentProjectile.BatchCreateCount);
		for (unsigned short i = 0; i < AttackCount; ++i)
			Attack_HorizontalRay(CurrentProjectile);
		return AttackCount;
	}
	return 0;
}


// Attack Calls
void BattleSystem::Attack_Bullet(EnemyProjectile& CurrentProjectile)
{
	// Spawn a bullet at the exterior of the GBox, fire it at the player.
	Vector3 SpawnPos = EnemyLayer->cUI_Layer[0]->TargetPosition - Vector3(EnemyLayer->cUI_Layer[0]->Dimensions.x * 0.5f);
	Vector3 TargetPos = Vector3(Math::RandFloatMinMax(BattleBox->cUI_Layer[0]->Position.x - BattleBox->cUI_Layer[0]->Dimensions.x, BattleBox->cUI_Layer[0]->Position.x + BattleBox->cUI_Layer[0]->Dimensions.x), CenterPosition.y * 2.5f);
	BattleScreenObject* BSO = GetInactiveBSO();
	BSO->SetParameters(CurrentProjectile.StoredMesh, 5, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	BSO->Type = BattleScreenObject::BS_Bullet;
	BSO->Active = true;
	BSO->TargetPoint = TargetPos;
	BSO->AltTargetPoint = PlayerObj->GetPosition() + 2 * Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale), 0);
	BSO->MoveToTarget = true;
	BSO->SetMass(3.f);
}

void BattleSystem::Attack_Trap(EnemyProjectile& CurrentProjectile)
{
	Vector3 SpawnPos = PlayerObj->GetPosition() + 2 * Vector3(Math::RandFloatMinMax(-PlayerScale, PlayerScale), Math::RandFloatMinMax(-PlayerScale, PlayerScale), 0);
	if (BattleBox->cUI_Layer[0]->UI_Bounds->CheckCollision(SpawnPos))
	{
		float RAngle = Math::RandFloatMinMax(-360.f, 360.f);
		BattleScreenObject* BSO = GetInactiveBSO();
		BSO->SetParameters("CrossMarker", 3, SpawnPos + Vector3(0, 0, -1), Vector3(PlayerScale, PlayerScale, 1), 0, RAngle, Vector3(0, 0, 1));
		BSO->Type = BattleScreenObject::BS_Null;
		BSO->Active = true;
		BSO->LifeTime = 1;
		BSO->SetAcceleration(0);
		BSO->MoveToTarget = false;
		BSO->AltTargetPoint = 0;

		BSO = GetInactiveBSO();
		BSO->SetParameters(CurrentProjectile.StoredMesh, 3, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), Vector3(CurrentProjectile.ScalarAcceleration, CurrentProjectile.ScalarAcceleration), RAngle, Vector3(0, 0, 1));
		CurrentProjectile.setName(BSO->GetMeshName());
		BSO->Type = BattleScreenObject::BS_Blast;
		BSO->Visible = false;
		BSO->Active = true;
		BSO->LifeTime = 4;
		BSO->SetMass(3.f);
		BSO->MoveToTarget = false;
		BSO->AltTargetPoint = 0;
	}
}

void BattleSystem::Attack_HorizontalRay(EnemyProjectile& CurrentProjectile)
{
	// Spawn a bullet at the exterior of the GBox, fire it at the player.
	Vector3 SpawnPos = EnemyLayer->cUI_Layer[0]->TargetPosition - Vector3(EnemyLayer->cUI_Layer[0]->Dimensions.x * 0.5f);
	Vector3 TargetPos = Vector3(Math::RandFloatMinMax(BattleBox->cUI_Layer[0]->Position.x - BattleBox->cUI_Layer[0]->Dimensions.x, BattleBox->cUI_Layer[0]->Position.x + BattleBox->cUI_Layer[0]->Dimensions.x), CenterPosition.y * 2.5f);
	Vector3 Target2 = Vector3(-CenterPosition.x * 0.2f, Math::RandFloatMinMax(BattleBox->cUI_Layer[0]->Position.y - BattleBox->cUI_Layer[0]->Dimensions.y * 0.5f, BattleBox->cUI_Layer[0]->Position.y + BattleBox->cUI_Layer[0]->Dimensions.y * 0.5f), 0);

	BattleScreenObject* BSO = GetInactiveBSO();
	BSO->SetParameters(CurrentProjectile.StoredMesh, 5, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	BSO->Type = BattleScreenObject::BS_HRay;
	BSO->Active = true;
	BSO->TargetPoint = TargetPos;
	BSO->AltTargetPoint = Target2;
	BSO->SetAcceleration(Vector3(CurrentProjectile.ScalarAcceleration));
	BSO->MoveToTarget = true;
	BSO->SetMass(2.f);

	BSO = GetInactiveBSO();
	BSO->SetParameters("BS_DangerZone", 3, Vector3(BattleBox->cUI_Layer[0]->Position.x, Target2.y), Vector3(BattleBox->cUI_Layer[0]->Dimensions.x, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	BSO->Type = BattleScreenObject::BS_Null;
	BSO->Active = true;
	BSO->LifeTime = 3;
	BSO->SetAcceleration(0);
	BSO->MoveToTarget = false;
	BSO->AltTargetPoint = 0;
}

void BattleSystem::Attack_VerticalRay(EnemyProjectile& CurrentProjectile)
{
	// Spawn a bullet at the exterior of the GBox, fire it at the player.
	Vector3 SpawnPos = EnemyLayer->cUI_Layer[0]->TargetPosition - Vector3(EnemyLayer->cUI_Layer[0]->Dimensions.x * 0.5f);
	Vector3 TargetPos = Vector3(Math::RandFloatMinMax(BattleBox->cUI_Layer[0]->Position.x - BattleBox->cUI_Layer[0]->Dimensions.x, BattleBox->cUI_Layer[0]->Position.x + BattleBox->cUI_Layer[0]->Dimensions.x), CenterPosition.y * 2.5f);
	Vector3 Target2 = Vector3(Math::RandFloatMinMax(BattleBox->cUI_Layer[0]->Position.x - BattleBox->cUI_Layer[0]->Dimensions.x * 0.5f, BattleBox->cUI_Layer[0]->Position.x + BattleBox->cUI_Layer[0]->Dimensions.x * 0.5f), 2.2f * CenterPosition.y, 0);

	BattleScreenObject* BSO = GetInactiveBSO();
	BSO->SetParameters(CurrentProjectile.StoredMesh, 5, SpawnPos, Vector3(PlayerScale, PlayerScale, 1), 0, 0, Vector3(0, 0, 1));
	BSO->Type = BattleScreenObject::BS_VRay;
	BSO->Active = true;
	BSO->TargetPoint = TargetPos;
	BSO->AltTargetPoint = Target2;
	BSO->SetAcceleration(Vector3(0, -CurrentProjectile.ScalarAcceleration));
	BSO->MoveToTarget = true;
	BSO->SetMass(2.f);

	BSO = GetInactiveBSO();
	BSO->SetParameters("BS_DangerZone", 3, Vector3(Target2.x, BattleBox->cUI_Layer[0]->Position.y), Vector3(PlayerScale, BattleBox->cUI_Layer[0]->Dimensions.y, 1), 0, 0, Vector3(0, 0, 1));
	BSO->Type = BattleScreenObject::BS_Null;
	BSO->Active = true;
	BSO->LifeTime = 3;
	BSO->SetAcceleration(0);
	BSO->MoveToTarget = false;
	BSO->AltTargetPoint = 0;
}