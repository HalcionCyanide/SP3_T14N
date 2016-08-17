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