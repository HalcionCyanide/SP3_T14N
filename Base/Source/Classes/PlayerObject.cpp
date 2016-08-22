#include "PlayerObject.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Mains\\Application.h"
#include "..\\Misc\\SimpleCommand.h"

PlayerObject::PlayerObject()
	: GameObject()
{
	m_bJumping = false;
	JumpVel = JUMPMAXSPEED = JUMPACCEL = 0;
	m_ElapsedTime = 0;
	theBoundaries = nullptr;
	GRAVITY = -180.0f;
	JumpVel = 0.0f;
	JUMPMAXSPEED = 60.0f;
	JUMPACCEL = 250.0f;
	m_bJumping = false;
	if (Bounds)
	{
		delete Bounds;
		Bounds = nullptr;
	}
}

PlayerObject::~PlayerObject()
{

}

void PlayerObject::Update(double dt)
{
	m_ElapsedTime = (float)dt;

	if (Application::IsKeyPressed(VK_SHIFT) &&
        !Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]))
	{
		MaxWalkSpeed = BaseWalkSpeed * 2;
	}
	else if (MaxWalkSpeed > BaseWalkSpeed)
	{
		MaxWalkSpeed -= BaseWalkSpeed * (float)dt;
		//Math::Clamp(CurrentWalkSpeed, 0.f, MaxWalkSpeed);
	}

	if (MovementValues.IsZero() == false)
	{
		if (theBoundaries)
		{
			Vector3 Xprediction;
			Xprediction.Set(GetPosition().x + MovementValues.x, GetPosition().y, GetPosition().z);
			Vector3 Zprediction;
			Zprediction.Set(GetPosition().x, GetPosition().y, GetPosition().z + MovementValues.z);
			//CheckBoundary here
			for (std::vector<GameObject*>::iterator it = theBoundaries->begin(); it != theBoundaries->end(); ++it)
			{
				if (MovementValues.IsEqual(0, MovementValues.x) == false && CheckCollision(*(*it)->GetBoundary(), Xprediction))
				{
					CheckCollision(*(*it)->GetBoundary(), Xprediction);
					MovementValues.x = 0;
				}
				if (MovementValues.IsEqual(0, MovementValues.z) == false && CheckCollision(*(*it)->GetBoundary(), Zprediction))
					MovementValues.z = 0;
				if (MovementValues.IsZero())
					break;
			}
		}
		SetPosition(GetPosition() + MovementValues);
		MovementValues.SetZero();
	}
	//if (!Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
	//	cameraObject->DecomposeMouseInertia(dt);
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND]))
	{
		Walk((float)dt);
	}
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]))
	{
		Walk(-(float)dt);
	}
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND]))
	{
		Strafe(-(float)dt);
	}
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND]))
	{
		Strafe((float)dt);
	}
    if (!CameraIsLocked &&Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::JUMP_COMMAND]))
	{
		Jump((float)dt);
	}
	if (m_bJumping == false)
	{
		Vector3 Pos = GetPosition();
		Pos.y = Application::cA_CurrentTerrainY;
		SetPosition(Pos);
	}
	if (!vel_.IsZero())
	{
		DecomposePlayerInertia((float)dt);
	}

	UpdateJump((float)dt);
}

void PlayerObject::SetJump(const float &speed, const float &max_speed, const float &accel)
{
	JumpVel = speed;
	JUMPMAXSPEED = max_speed;
	JUMPACCEL = accel;
}

void PlayerObject::setVel(const Vector3 &theSpeed)
{
	vel_ = theSpeed;
}

void PlayerObject::setAccel(const Vector3 &theacceleration)
{
	accel_ = theacceleration;
}

void PlayerObject::walkDirection(const float &degree, const float &byHowMuch)
{
	MovementValues.x = (float)(sin(Math::DegreeToRadian(GetRotationAngle() + degree)) * vel_.x * (float)(m_ElapsedTime)* byHowMuch);
	MovementValues.z = (float)(cos(Math::DegreeToRadian(GetRotationAngle() + degree)) * vel_.x * (float)(m_ElapsedTime)* byHowMuch);
}

void PlayerObject::setPlayerBoundaries(std::vector<GameObject*> &Playerboundary)
{
	theBoundaries = &Playerboundary;
}

bool PlayerObject::CheckCollision(const Boundary &object, const Vector3 &Prediction)
{
	return object.CheckCollision(Prediction, this->MovementValues);
}

void PlayerObject::DecomposePlayerInertia(float dt)
{
	float NegligibleVelocity = 0.3f;
	float RateOfDecomposition = 2.f;

	if (abs(vel_.x) > 0 && abs(vel_.x) <= NegligibleVelocity)
		vel_.x = 0;
	if (abs(vel_.z) > 0 && abs(vel_.z) <= NegligibleVelocity)
		vel_.z = 0;
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND])){
		if (vel_.x <= 0){
			vel_.x += MaxWalkSpeed * RateOfDecomposition * (float)dt;
			P_BackwardMovement(dt);
		}
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND])){
		if (vel_.x >= 0){
			vel_.x -= MaxWalkSpeed * RateOfDecomposition * (float)dt;
			P_ForwardMovement(dt);
		}
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND])){
		if (vel_.z <= 0){
			vel_.z += MaxWalkSpeed * RateOfDecomposition * (float)dt;
			P_LeftMovement(dt);
		}
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND])){
		if (vel_.z >= 0){
			vel_.z -= MaxWalkSpeed * RateOfDecomposition * (float)dt;
			P_RightMovement(dt);
		}
	}
}

void PlayerObject::Walk(const float dt)
{
	if (dt > 0)
		P_ForwardMovement(dt);
	else if (dt < 0)
		P_BackwardMovement(abs(dt));
}
void PlayerObject::Strafe(const float dt)
{
	if (dt > 0)
		P_RightMovement(dt);
	else if (dt < 0)
		P_LeftMovement(abs(dt));
}

void PlayerObject::Jump(const float dt)
{
	if (m_bJumping == false)
	{
		m_bJumping = true;

		// Calculate the jump velocity
		JumpVel = JUMPACCEL;

		// Factor in maximum speed limit
		if (JumpVel > JUMPMAXSPEED)
			JumpVel = JUMPMAXSPEED;
	}
}

void PlayerObject::UpdateJump(const float dt)
{
	if (m_bJumping == true)
	{
		// Factor in gravity
		JumpVel += GRAVITY * dt;

		// Update the camera and target position
		SetPosition(GetPosition() + Vector3(0, JumpVel * (float)dt));

		// Check if the camera has reached the ground
		if (GetPosition().y <= Application::cA_MinimumTerrainY)
		{
			Vector3 Pos = GetPosition();
			Pos.y = Application::cA_CurrentTerrainY;
			SetPosition(Pos);
			JumpVel = 0.0f;
			m_bJumping = false;
		}
	}
}

void PlayerObject::P_ForwardMovement(const float dt)
{
	//Velocity = Acceleration x Time
	vel_.x += MaxWalkSpeed * (float)dt;
	if (vel_.x > MaxWalkSpeed) { vel_.x = MaxWalkSpeed; }
	MovementValues.x += (float)(sin(Math::DegreeToRadian(GetRotationAngle())) * (float)dt * vel_.x);
	MovementValues.z += (float)(cos(Math::DegreeToRadian(GetRotationAngle())) * (float)dt * vel_.x);
}
void PlayerObject::P_BackwardMovement(const float dt)
{
	vel_.x -= MaxWalkSpeed * (float)dt;
	if (vel_.x < -MaxWalkSpeed) { vel_.x = -MaxWalkSpeed; }
	MovementValues.x -= (float)(sin(Math::DegreeToRadian(GetRotationAngle() + 180)) * (float)dt * vel_.x);
	MovementValues.z -= (float)(cos(Math::DegreeToRadian(GetRotationAngle() + 180)) * (float)dt * vel_.x);
}
void PlayerObject::P_LeftMovement(const float dt)
{
	vel_.z -= MaxWalkSpeed * (float)dt;
	if (vel_.z < -MaxWalkSpeed) { vel_.z = -MaxWalkSpeed; }
	MovementValues.x -= (float)(sin(Math::DegreeToRadian(GetRotationAngle() + 90)) * (float)dt * vel_.z);
	MovementValues.z -= (float)(cos(Math::DegreeToRadian(GetRotationAngle() + 90)) * (float)dt * vel_.z);
}
void PlayerObject::P_RightMovement(const float dt)
{
	vel_.z += MaxWalkSpeed * (float)dt;
	if (vel_.z > MaxWalkSpeed) { vel_.z = MaxWalkSpeed; }
	MovementValues.x += (float)(sin(Math::DegreeToRadian(GetRotationAngle() + 270)) * (float)dt * vel_.z);
	MovementValues.z += (float)(cos(Math::DegreeToRadian(GetRotationAngle() + 270)) * (float)dt * vel_.z);
}