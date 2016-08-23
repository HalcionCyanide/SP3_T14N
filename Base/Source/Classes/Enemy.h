#ifndef _ENEMY_H
#define _ENEMY_H

#include "Mesh.h"
#include "GenericEntity.h"

// Stats Container for an Enemy Object
struct Enemy : public GenericEntity
{
	Enemy(){
		CurrentTime = 0;
		CurrentAttackCount = 0;
		CurrentEnemyWave = 0;
	};
	~Enemy(){};

	std::string MeshName;

	int SpellPower;

	float AttackSpeed;
	int AttacksPerWave;
	int CurrentAttackCount;

	float CurrentTime;
	float DamagePerAttack;

	int MaxEnemyWave;
	int CurrentEnemyWave;

	static const unsigned short MAX_MESHES = 4;
	Mesh* StoredMeshes[MAX_MESHES];
	std::string ProjectileTypes[MAX_MESHES - 1];

};

#endif //_ENEMY_H