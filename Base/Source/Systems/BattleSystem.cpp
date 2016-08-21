#include "BattleSystem.h"
#include "Scene_System.h"
#include <string>
#include "../Classes/Boundary2D.h"

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
	CursorPosition = Scene_System::accessing().cSS_InputManager->GetMousePosition();

	// Note Init Player and Enemy Here
	CenterPosition.Set(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	PlayerScale = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.03f;
	Player = new BaseObject("Player", 3, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), Vector3(0, 0, 0), 0, Vector3(0, 0, 1));

	// Background If Needed
	BaseExterior = new BaseObject("GBox", 0.f, CenterPosition + Vector3(0, CenterPosition.y * 0.1f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.65f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.90f, 1), Vector3(), 0, Vector3(0, 0, 1));
	BaseInterior = new BaseObject("GBox", 0.f, CenterPosition + Vector3(0, CenterPosition.y * 0.1f), Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.55f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.75f, 1), Vector3(), 0, Vector3(0, 0, 1));


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
	BaseExterior->Render();
	BaseInterior->Render();
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
	Player->Update(dt);
}

void BattleSystem::UpdateITimer(float dt)
{

}

void BattleSystem::UpdateControls(float dt)
{
	Boundary2D TempBounds;
	TempBounds.CalculateValues(BaseInterior->GetPosition(), BaseInterior->GetDimensions());
	if (TempBounds.CheckCollision(Scene_System::accessing().cSS_InputManager->GetMousePosition(), Vector3()))
	{
		CursorPosition = Scene_System::accessing().cSS_InputManager->GetMousePosition();
	}
	float ForceIncrement = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f;
	switch (MouseModeSelected)
	{
	case (true) :
	{
		Vector3 PlayerDirection = (CursorPosition - Player->GetPosition());
		if (!PlayerDirection.IsZero())
			if (PlayerDirection.LengthSquared() < (Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.15f))
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
	if (Player->GetMass() > Math::EPSILON)
	{
		// Velocity Due To Acceleration If Mass Exists
		Player->SetVelocity(Player->GetVelocity() + PlayerActingForce * (1.f / Player->GetMass()) * dt);
	}
	Player->SetVelocity(Player->GetVelocity() - Player->GetVelocity()*FrictionDecrementMultiplier * dt);
	Vector3 FwdPos = Player->GetPosition() + Player->GetVelocity() * dt;
	// <!> Needs work
	if (abs(FwdPos.x - TempBounds.GetPosition().x) > TempBounds.GetScale().x * 0.5f - Player->GetDimensions().x * 0.5f)
		if (abs(Player->GetVelocity().x * 0.5f) < Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.01f)
			Player->SetVelocity(Vector3(0, Player->GetVelocity().y, 0.f));
		else Player->SetVelocity(Vector3(-Player->GetVelocity().x * 0.5f, Player->GetVelocity().y, 0.f));
		if (abs(FwdPos.y - TempBounds.GetPosition().y) > TempBounds.GetScale().y * 0.5f - Player->GetDimensions().y * 0.5f)
			if (abs(Player->GetVelocity().y * 0.5f) < Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.01f)
				Player->SetVelocity(Vector3(Player->GetVelocity().x, 0, 0.f));
			else Player->SetVelocity(Vector3(Player->GetVelocity().x, -Player->GetVelocity().y * 0.5f, 0.f));

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
