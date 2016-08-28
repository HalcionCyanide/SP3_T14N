/****************************************************************************/
/*!
\file BattleScreenObject.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines a battle screen object that exists within the game
*/
/****************************************************************************/

#ifndef _BATTLESCREENOBJECT_H
#define _BATTLESCREENOBJECT_H

#include "BaseObject.h"

class BattleScreenObject : public BaseObject
{
public:
	enum BS_OBJECT
	{
		BS_Null,
		BS_Normal,
		BS_Bullet,
		BS_ScatterShot,
		BS_Blast,
		BS_Chaser,
	};

	BattleScreenObject(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Acceleration, const float& RotationAngle, const Vector3& RotationAxis);
	virtual ~BattleScreenObject();

	virtual void Init(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Acceleration, const float& RotationAngle, const Vector3& RotationAxis);
	virtual void Update(double dt);

	void SetAcceleration(const Vector3&);
	Vector3 GetAcceleration();

	BS_OBJECT Type = BS_Normal;
	float LifeTime = -1;
	float LifeTimer = 0;
	bool GravityAffected = false;
	Vector3 TargetPoint;
	bool MoveToTarget = false;
	Vector3 AltTargetPoint;

private:
	Vector3 Acceleration;
};

#endif // _BATTLESCREENOBJECT_H