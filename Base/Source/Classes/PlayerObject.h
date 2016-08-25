#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "Camera3.h"
#include "GameObject.h"
#include <vector>

class PlayerObject : public GameObject
{
public:
    PlayerObject();
    virtual ~PlayerObject();

	//Camera3* cameraObject = nullptr;

    virtual void Update(double dt);

    virtual void SetJump(const float &speed, const float &max_speed, const float &accel);
    virtual void setAccel(const Vector3 &theacceleration);

    virtual void setPlayerBoundaries(std::vector<GameObject*> &Playerboundary);
	virtual bool CheckCollision(Boundary &object, const Vector3 &Prediction);
	virtual bool CheckCollision(Boundary &object, Boundary &Prediction);

private:
	void DecomposePlayerInertia(float dt);
	void CameraTiltMotion(double dt, bool Moving);

	virtual void P_ForwardMovement(const float dt);
	virtual void P_BackwardMovement(const float dt);
	virtual void P_LeftMovement(const float dt);
	virtual void P_RightMovement(const float dt);
	virtual void Walk(const float dt);
	virtual void Strafe(const float dt);
	virtual void Jump(const float dt);
	virtual void UpdateJump(const float dt);

protected:
    virtual void walkDirection(const float &degree, const float &byHowMuch);

	float BaseWalkSpeed = 16.0f;
	float MaxWalkSpeed = 32.0f;

	bool CameraIsLocked = false;

    Vector3 accel_, MovementValues;
    float JumpVel;
	float GRAVITY;
    float JUMPMAXSPEED, JUMPACCEL;
    bool m_bJumping = false;
    double m_ElapsedTime;
    std::vector<GameObject*> *theBoundaries;
};


#endif