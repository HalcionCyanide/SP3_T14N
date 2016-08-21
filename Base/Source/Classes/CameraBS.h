/****************************************************************************/
/*!
\file CameraBS.h
\author Ryan Lim Rui An
\par email: 150577@.mymail.nyp.edu.sg
\brief
Class to define the battle screen camera
*/
/****************************************************************************/

#ifndef CAMERA_BS_H
#define CAMERA_BS_H

#include "Camera.h"
#include "..\\Systems\\Scene_System.h"
#include <vector>

class CameraBS : public Camera
{
public:
	CameraBS();
	virtual ~CameraBS();

	virtual void Init(const Vector3& Position, const Vector3& Target, const Vector3& Up);
	virtual void Update(float dt);
	virtual void Reset();

	void ShakeCamera(const float& Strength);

private:
	Vector3 View;

	Vector3 DefaultTarget;
	Vector3 DefaultPosition;
	Vector3 DefaultUp;

	Vector3 CameraVelocity;

	const float CameraSpeed = 10.f;

protected:

};

#endif // CAMERA_BS_H