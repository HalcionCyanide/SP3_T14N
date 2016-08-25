/****************************************************************************/
/*!
\file Camera3.cpp
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
.cpp file that defines camera
*/
/****************************************************************************/

#include "Camera3.h"
#include "..\\Mains\\Application.h"
#include "Mtx44.h"
#include "..\Misc\SimpleCommand.h"

/****************************************************************************/
/*!
\brief
Constructor for Camera
\return N/A
*/
/****************************************************************************/

Camera3::Camera3()
	: CameraVelocity(0, 0, 0)
{
}

/****************************************************************************/
/*!
\brief
Destructor for Camera
\return N/A
*/
/****************************************************************************/

Camera3::~Camera3()
{
}

/****************************************************************************/
/*!
\brief
Initializes variables
\param
position - current position of the camera
target - current target of the camera
view - current direction vector of the camera.
right - current right vector of the camera.

CurrentCameraRotation - camera's current rotation angles for xyz axes
MinimumCameraRotation - minimum possible rotation angles for xyz axes
MaximumCameraRotation - minimum possible rotation angles for xyz axes
RelativeMouseSensitivity - mouse sensitivity value, used to calculate camera rotation speed
CameraBaseWalkSpeed - camera normal walkspeed
CameraCurrentWalkSpeed - camera current walkspeed
CameraMaxWalkSpeed - camera max walkspeed

*/
/****************************************************************************/
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	MinimumCameraRotation.Set(-89, -360, -35);
	MaximumCameraRotation.Set(89, 360, 35);
	CurrentCameraRotation.SetZero();
	RelativeMouseSensitivity = 3;

	CameraBaseWalkSpeed = CameraMaxWalkSpeed = CameraCurrentWalkSpeed = 16.0f;

	// For Jump use
	m_bJumping = false;
	JumpVel = 0.0f;
	JUMPMAXSPEED = 60.0f;
	JUMPACCEL = 250.0f;
	GRAVITY = -180.0f;
}

/****************************************************************************/
/*!
\brief
Method where values are changed
\param

*/
/****************************************************************************/
void Camera3::Update(float dt)
{
	CameraRotationSpeed = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * RelativeMouseSensitivity;
	Application::cA_CurrentTerrainY += CameraBobVal;
	MovementValues.SetZero();

	if (Application::IsKeyPressed(VK_SHIFT) &&
		!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]))
	{
		CameraMaxWalkSpeed = CameraBaseWalkSpeed * 2;
	}
	else if (CameraMaxWalkSpeed > CameraBaseWalkSpeed)
	{
		CameraMaxWalkSpeed -= CameraBaseWalkSpeed * (float)dt;
		Math::Clamp(CameraCurrentWalkSpeed, 0.f, CameraMaxWalkSpeed);
	}
	if (!CameraVelocity.IsZero())
	{
		DecomposePlayerInertia(dt);
	}
	if (!Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
		DecomposeMouseInertia(dt);

    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND]))
	{
		Walk(dt);
	}
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]))
	{
		Walk(-dt);
	}
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND]))
	{
		Strafe(-dt);
	}
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND]))
	{
		Strafe(dt);
	}
    if (!CameraIsLocked &&Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::JUMP_COMMAND]))
	{
		Jump(dt);
	}

	if (m_bJumping == false)
	{
		position.y = Application::cA_CurrentTerrainY;
	}

	//Update Camera Values
	UpdateCameraAngles(dt);
	UpdateJump(dt);
	UpdateCameraPosition();
	UpdateCameraVectors();

	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('R'))
	{
		Reset();
	}
}

void Camera3::DecomposePlayerInertia(float dt)
{
	float NegligibleVelocity = 0.5f;
	float RateOfDecomposition = 2.f;
	if (abs(CameraVelocity.x) > 0 && abs(CameraVelocity.x) <= NegligibleVelocity)
	{
		CameraVelocity.x = 0;
	}
	if (abs(CameraVelocity.z) > 0 && abs(CameraVelocity.z) <= NegligibleVelocity)
	{
		CameraVelocity.z = 0;
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND])){
		if (CameraVelocity.x <= 0){
			CameraVelocity.x += CameraMaxWalkSpeed * RateOfDecomposition * (float)dt;
			C_BackwardMovement(dt);
		}
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND])){
		if (CameraVelocity.x >= 0){
			CameraVelocity.x -= CameraMaxWalkSpeed * RateOfDecomposition * (float)dt;
			C_ForwardMovement(dt);
		}
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND])){
		if (CameraVelocity.z <= 0){
			CameraVelocity.z += CameraMaxWalkSpeed * RateOfDecomposition * (float)dt;
			C_LeftMovement(dt);
		}
	}
    if (!Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND])){
		if (CameraVelocity.z >= 0){
			CameraVelocity.z -= CameraMaxWalkSpeed * RateOfDecomposition * (float)dt;
			C_RightMovement(dt);
		}
	}
}

void Camera3::DecomposeMouseInertia(float dt)
{
	float C_Rot_Accel = 0.5f;
	float MaxRotSpeed = C_Rot_Accel;// *1.5f;
	Yaw_Velocity += C_Rot_Accel * dt * Scene_System::accessing().cSS_InputManager->cIM_CameraYaw;
	Yaw_Velocity = Math::Clamp(Yaw_Velocity, -MaxRotSpeed, MaxRotSpeed);
	if (!CameraIsLocked && Yaw_Velocity) {
		Yaw(dt);
		// Remove the residual velocity after some time
		Yaw_Velocity -= Yaw_Velocity * dt;
	}

	Pitch_Velocity += C_Rot_Accel * dt * Scene_System::accessing().cSS_InputManager->cIM_CameraPitch;
	Pitch_Velocity = Math::Clamp(Pitch_Velocity, -MaxRotSpeed, MaxRotSpeed);
	if (!CameraIsLocked && Pitch_Velocity){
		Pitch(dt);
		// Remove the residual velocity after some time
		Pitch_Velocity -= Pitch_Velocity * dt;
	}
}

void Camera3::CameraTiltMotion(double dt, bool Moving)
{
	//Values halved for testing
	float CamTiltSpeed = CameraCurrentWalkSpeed / 13.f;
	float TiltAmount = CameraCurrentWalkSpeed / 70.f;
	float BobSpeed = CamTiltSpeed / 10;
	float BobAmount = (CameraCurrentWalkSpeed / 700.f);
	if (Moving && cos(Math::DegreeToRadian(abs(CurrentCameraRotation.x))) > 0.3f)
	{
		if (!TiltDir && CurrentCameraRotation.z >= TiltAmount){ TiltDir = true; }
		else if (TiltDir && CurrentCameraRotation.z <= -TiltAmount){ TiltDir = false; }

		if (TiltDir && CurrentCameraRotation.z > -TiltAmount)
		{
			CurrentCameraRotation.z -= (float)(dt * CamTiltSpeed);
		}
		else if (!TiltDir && CurrentCameraRotation.z < TiltAmount)
		{
			CurrentCameraRotation.z += (float)(dt * CamTiltSpeed);
		}

		if (!BobDir && CameraBobVal >= BobAmount){ BobDir = true; }
		else if (BobDir && CameraBobVal <= -BobAmount){ BobDir = false; }

		if (BobDir && CameraBobVal > -BobAmount)
		{
			CameraBobVal -= (float)(dt * BobSpeed);
		}
		else if (!BobDir && CameraBobVal < BobAmount)
		{
			CameraBobVal += (float)(dt * BobSpeed);
		}
	}
	else if (CurrentCameraRotation.z)
	{
		float interval = CurrentCameraRotation.z / CamTiltSpeed * 1.25f; //Interval of reset
		CurrentCameraRotation.z -= interval;
		interval = CameraBobVal / CamTiltSpeed * 1.5f; //Interval of reset
		CameraBobVal -= interval;

	}
}

void Camera3::Reset()
{
	Init(defaultPosition, defaultTarget, defaultUp);
}

void Camera3::UpdateCameraAngles(double dt)
{
	if (!CameraIsLocked)
		CameraTiltMotion(dt, MovementValues.x || MovementValues.y || MovementValues.z);
	CurrentCameraRotation.x = Math::Clamp(CurrentCameraRotation.x, MinimumCameraRotation.x, MaximumCameraRotation.x);
	CurrentCameraRotation.z = Math::Clamp(CurrentCameraRotation.z, MinimumCameraRotation.z, MaximumCameraRotation.z);
	if (MaximumCameraRotation.y < 360 && MinimumCameraRotation.y < 360)
	{
		CurrentCameraRotation.y = Math::Clamp(CurrentCameraRotation.y, MinimumCameraRotation.y, MaximumCameraRotation.y);
	}
}

void Camera3::UpdateCameraVectors()
{
	target = Vector3(sin(Math::DegreeToRadian(CurrentCameraRotation.y)) * cos(Math::DegreeToRadian(CurrentCameraRotation.x)) + position.x,
		-sin(Math::DegreeToRadian(CurrentCameraRotation.x)) + position.y,
		cos(Math::DegreeToRadian(CurrentCameraRotation.y)) * cos(Math::DegreeToRadian(CurrentCameraRotation.x)) + position.z);
	view = (target - position).Normalized();

	up = Vector3(sin(Math::DegreeToRadian(CurrentCameraRotation.z)) * cos(Math::DegreeToRadian(CurrentCameraRotation.z)) * cos(Math::DegreeToRadian(CurrentCameraRotation.y)),
		cos(Math::DegreeToRadian(CurrentCameraRotation.x)),
		sin(Math::DegreeToRadian(CurrentCameraRotation.z)) * cos(Math::DegreeToRadian(CurrentCameraRotation.z)) * sin(Math::DegreeToRadian(-CurrentCameraRotation.y)));

	Vector3 right = view.Cross(up);
	up = right.Cross(view);
}

void Camera3::UpdateCameraPosition()
{
	if (!CameraIsLocked)
		position += MovementValues;
}


void Camera3::C_ForwardMovement(const float dt)
{
	//Velocity = Acceleration x Time
	CameraVelocity.x += CameraMaxWalkSpeed * (float)dt;
	if (CameraVelocity.x > CameraMaxWalkSpeed) { CameraVelocity.x = CameraMaxWalkSpeed; }
	MovementValues.x += (float)(sin(Math::DegreeToRadian(CurrentCameraRotation.y)) * (float)dt * CameraVelocity.x);
	MovementValues.z += (float)(cos(Math::DegreeToRadian(CurrentCameraRotation.y)) * (float)dt * CameraVelocity.x);
}
void Camera3::C_BackwardMovement(const float dt)
{
	CameraVelocity.x -= CameraMaxWalkSpeed * (float)dt;
	if (CameraVelocity.x < -CameraMaxWalkSpeed) { CameraVelocity.x = -CameraMaxWalkSpeed; }
	MovementValues.x -= (float)(sin(Math::DegreeToRadian(CurrentCameraRotation.y + 180)) * (float)dt * CameraVelocity.x);
	MovementValues.z -= (float)(cos(Math::DegreeToRadian(CurrentCameraRotation.y + 180)) * (float)dt * CameraVelocity.x);
}
void Camera3::C_LeftMovement(const float dt)
{
	CameraVelocity.z -= CameraMaxWalkSpeed * (float)dt;
	if (CameraVelocity.z < -CameraMaxWalkSpeed) { CameraVelocity.z = -CameraMaxWalkSpeed; }
	MovementValues.x -= (float)(sin(Math::DegreeToRadian(CurrentCameraRotation.y + 90)) * (float)dt * CameraVelocity.z);
	MovementValues.z -= (float)(cos(Math::DegreeToRadian(CurrentCameraRotation.y + 90)) * (float)dt * CameraVelocity.z);
}
void Camera3::C_RightMovement(const float dt)
{
	CameraVelocity.z += CameraMaxWalkSpeed * (float)dt;
	if (CameraVelocity.z > CameraMaxWalkSpeed) { CameraVelocity.z = CameraMaxWalkSpeed; }
	MovementValues.x += (float)(sin(Math::DegreeToRadian(CurrentCameraRotation.y + 270)) * (float)dt * CameraVelocity.z);
	MovementValues.z += (float)(cos(Math::DegreeToRadian(CurrentCameraRotation.y + 270)) * (float)dt * CameraVelocity.z);
}

/********************************************************************************
Turn left
********************************************************************************/
void Camera3::TurnLeft(const float dt)
{
	CurrentCameraRotation.y += Yaw_Velocity * Scene_System::accessing().cSS_InputManager->cIM_CameraYaw * (float)(dt)* CameraRotationSpeed;
}

/********************************************************************************
Turn right
********************************************************************************/
void Camera3::TurnRight(const float dt)
{
	CurrentCameraRotation.y -= Yaw_Velocity * Scene_System::accessing().cSS_InputManager->cIM_CameraYaw * (float)(dt)* CameraRotationSpeed;
}

/********************************************************************************
LookUp
********************************************************************************/
void Camera3::LookUp(const float dt)
{
	CurrentCameraRotation.x += Scene_System::accessing().cSS_InputManager->cIM_CameraPitch * (float)(dt)* CameraRotationSpeed;
}
/********************************************************************************
LookDown
********************************************************************************/
void Camera3::LookDown(const float dt)
{
	CurrentCameraRotation.x += Scene_System::accessing().cSS_InputManager->cIM_CameraPitch * (float)(dt)* CameraRotationSpeed;
}

/********************************************************************************
Pitch. You can add in a deadzone here.
********************************************************************************/
void Camera3::Pitch(const float dt)
{
	if (Scene_System::accessing().cSS_InputManager->cIM_CameraPitch > 0.0)
		LookUp(dt);
	else if (Scene_System::accessing().cSS_InputManager->cIM_CameraPitch < 0.0)
		LookDown(dt);
}
/********************************************************************************
Yaw. You can add in a deadzone here.
********************************************************************************/
void Camera3::Yaw(const float dt)
{
	if (Scene_System::accessing().cSS_InputManager->cIM_CameraYaw > 0.0)
		TurnRight(dt);
	else if (Scene_System::accessing().cSS_InputManager->cIM_CameraYaw < 0.0)
		TurnLeft(dt);
}
/********************************************************************************
Walk forward or backward. You can add in a deadzone here.
********************************************************************************/
void Camera3::Walk(const float dt)
{
	if (dt > 0)
		C_ForwardMovement(dt);
	else if (dt < 0)
		C_BackwardMovement(abs(dt));
}
/********************************************************************************
Strafe left or right. You can add in a deadzone here.
********************************************************************************/
void Camera3::Strafe(const float dt)
{
	if (dt > 0)
		C_RightMovement(dt);
	else if (dt < 0)
		C_LeftMovement(abs(dt));
}
/********************************************************************************
Jump
********************************************************************************/
void Camera3::Jump(const float dt)
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

/********************************************************************************
Update Jump
********************************************************************************/
void Camera3::UpdateJump(const float dt)
{
	if (m_bJumping == true)
	{
		// Factor in gravity
		JumpVel += GRAVITY * dt;

		// Update the camera and target position
		position.y += JumpVel * (float)dt;
		target.y += JumpVel * (float)dt;

		// Check if the camera has reached the ground
		if (position.y <= Application::cA_MinimumTerrainY)
		{
			position.y = target.y = Application::cA_CurrentTerrainY;
			JumpVel = 0.0f;
			m_bJumping = false;
		}
	}
}