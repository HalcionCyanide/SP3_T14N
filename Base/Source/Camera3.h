/****************************************************************************/
/*!
\file Camera3.h
\author Ryan Lim Rui An
\par email: 150577@.mymail.nyp.edu.sg
\brief
Class to define camera
*/
/****************************************************************************/

#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "Scene_System.h"
#include <vector>

/****************************************************************************/
/*!
Class Camera3:
\brief Defines the methods for camera control
*/
/****************************************************************************/
class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(float dt);
	virtual void Reset();

	const float PlayerHeight = 10.f;

	float CameraCurrentWalkSpeed = 16.f;

	Vector3 CurrentCameraRotation;

	Vector3 CameraVelocity;

	bool CameraIsLocked = false;

	void UpdateCameraVectors();
private:

	Vector3 view;

	float RelativeMouseSensitivity = 1;

	float CameraBaseWalkSpeed = 16.0f;
	float CameraMaxWalkSpeed = 32.0f;

	void UpdateCameraAngles(double dt);
	void UpdateCameraPosition();
	Vector3 MinimumCameraRotation;
	Vector3 MaximumCameraRotation;
	Vector3 MovementValues = Vector3(0, 0, 0);
	float CameraRotationSpeed;

	// For Jump use
	bool m_bJumping;
	float GRAVITY;
	float JumpVel;
	float JUMPMAXSPEED, JUMPACCEL;

	// For Inertia
	float Yaw_Velocity = 0.f;
	float Pitch_Velocity = 0.f;
	float InertiaTimeLimit = 0;
	void DecomposePlayerInertia(float dt);
	void DecomposeMouseInertia(float dt);

	// For camera animations
	bool TiltDir = false;
	bool BobDir = false;
	float CameraBobVal = 0.f;
	void CameraTiltMotion(double dt, bool Moving);

	// Camera methods
	virtual void C_ForwardMovement(const float dt);
	virtual void C_BackwardMovement(const float dt);
	virtual void C_LeftMovement(const float dt);
	virtual void C_RightMovement(const float dt);
	virtual void TurnLeft(const float dt);
	virtual void TurnRight(const float dt);
	virtual void LookUp(const float dt);
	virtual void LookDown(const float dt);
	virtual void Pitch(const float dt);
	virtual void Yaw(const float dt);
	virtual void Walk(const float dt);
	virtual void Strafe(const float dt);
	virtual void Jump(const float dt);
	virtual void UpdateJump(const float dt);
};

#endif