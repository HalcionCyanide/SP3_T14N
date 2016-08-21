#include "CameraBS.h"
#include "..\\Mains\\Application.h"

CameraBS::CameraBS()
{

}

CameraBS::~CameraBS()
{

}


void CameraBS::Init(const Vector3& Position, const Vector3& Target, const Vector3& Up)
{
	// Camera Base Init
	this->position = DefaultPosition = Position;
	this->target = DefaultTarget = Target;
	this->View = (target - position).Normalized();
	Vector3 right = View.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = DefaultUp = right.Cross(View).Normalized();

	// Init Variables
	CameraVelocity.SetZero();
}

void CameraBS::Update(float dt)
{
	// <!> Side Note - Camera Shake not possible in ortho
	Vector3 DirectionVector = (DefaultPosition - position);
	if (!DirectionVector.IsZero())
	{
		if (DirectionVector.LengthSquared() < (Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.15f))
			DirectionVector.SetZero();
		else DirectionVector.Normalize();
	}
	//position += CameraVelocity * dt;
	CameraVelocity -= CameraVelocity * 0.9f * dt;
}

void CameraBS::Reset()
{

}

void CameraBS::ShakeCamera(const float& Strength)
{
	CameraVelocity += Vector3(Math::RandFloatMinMax(-Strength, Strength), Math::RandFloatMinMax(-Strength, Strength));
}
