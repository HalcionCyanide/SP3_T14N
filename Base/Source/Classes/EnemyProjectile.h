/****************************************************************************/
/*!
\file EnemyProjectile.h
\author Ryan Lim Rui An, Lee Sek Heng
\par email: 150577L@mymail.nyp.edu.sg, 150629Z@mymail.nyp.edu.sg
\brief
Defines an Enemy Projectile
*/
/****************************************************************************/


#ifndef _ENEMY_PROJECTILE_H
#define _ENEMY_PROJECTILE_H

#include "Mesh.h"
#include "GenericEntity.h"

struct EnemyProjectile : public GenericEntity
{

	Mesh* StoredMesh;

	std::string AttackType;
	float ScalarAcceleration;
	float AttackSpeed;
	int AttacksPerWave;
	int BatchCreateCount;
	int DamagePerAttack;

	EnemyProjectile &operator=(EnemyProjectile &rhs)
	{
		StoredMesh = rhs.StoredMesh;
		AttackType = rhs.AttackType;
		ScalarAcceleration = rhs.ScalarAcceleration;
		AttackSpeed = rhs.AttackSpeed;
		AttacksPerWave = rhs.AttacksPerWave;
		BatchCreateCount = rhs.BatchCreateCount;
		DamagePerAttack = rhs.DamagePerAttack;
		return *this;
	}
	EnemyProjectile(EnemyProjectile &rhs)
	{
		*this = rhs;
	}
	EnemyProjectile()
	{
		StoredMesh = nullptr;
		ScalarAcceleration = AttackSpeed = 0;
		AttacksPerWave = BatchCreateCount = DamagePerAttack = 0;
	}
	virtual ~EnemyProjectile() {};
};

#endif // _ENEMY_PROJECTILE_H