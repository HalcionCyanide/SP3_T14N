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
};

#endif // _ENEMY_PROJECTILE_H