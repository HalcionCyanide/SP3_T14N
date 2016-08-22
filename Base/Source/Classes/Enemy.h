#ifndef _ENEMY_H
#define _ENEMY_H

#include "Mesh.h"
#include "GenericEntity.h"

// Stats Container for an Enemy Object
struct Enemy : public GenericEntity
{
	Enemy(){};
	~Enemy(){};

	std::string MeshName;

	int SpellPower = 1;

	float AttackSpeed = 1;
	int AttacksPerWave = 10;
	int CurrentAttackCount = 0;

	float CurrentTime = 0;
	float DamagePerAttack = 1;

	int MaxEnemyWave = 100;
	int CurrentEnemyWave = 0;
	static const unsigned short MAX_MESHES = 4;
	Mesh* StoredMeshes[MAX_MESHES];
	std::string ProjectileTypes[MAX_MESHES - 1];

};

#endif //_ENEMY_H