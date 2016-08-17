#include "PlayerObject.h"
#include "Scene_System.h"

PlayerObject::PlayerObject()
    : GameObject()
{
    m_bJumping = false;
    JumpVel = JUMPMAXSPEED = JUMPACCEL = 0;
}

PlayerObject::~PlayerObject()
{

}

void PlayerObject::Update(double dt)
{
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('W'))
    {

    }
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('S'))
    {

    }
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('A'))
    {

    }
    if (Scene_System::accessing().cSS_InputManager->GetKeyValue('D'))
    {

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

}