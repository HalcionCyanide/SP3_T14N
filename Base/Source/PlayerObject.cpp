#include "PlayerObject.h"
#include "Scene_System.h"

PlayerObject::PlayerObject()
    : GameObject()
{
    m_bJumping = false;
    JumpVel = JUMPMAXSPEED = JUMPACCEL = 0;
    m_ElapsedTime = 0;
    theBoundaries = nullptr;
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
    m_ElapsedTime = dt;
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('W'))
    {
        walkDirection(0, 1.f);
    }
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('S'))
    {
        walkDirection(180.f, 1.f);
    }
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('A'))
    {
        walkDirection(90.f, 1.f);
    }
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('D'))
    {
        walkDirection(270.f, 1.f);
    }

    if (MovementValues.IsZero() == false)
    {
        MovementValues.SetZero();
    }
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
    MovementValues.x += (float)(sin(Math::DegreeToRadian(RotationAngle + degree)) * vel_.x * m_ElapsedTime * byHowMuch);
    MovementValues.z += (float)(cos(Math::DegreeToRadian(RotationAngle + degree)) * vel_.x * m_ElapsedTime * byHowMuch);
}

void PlayerObject::setPlayerBoundaries(std::vector<GameObject*> &Playerboundary)
{
    theBoundaries = &Playerboundary;
}