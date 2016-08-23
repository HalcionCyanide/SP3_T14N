#ifndef _ENEMY_H
#define _ENEMY_H

#include "EnemyProjectile.h"
#include <vector>

// Stats Container for an Enemy Object
struct Enemy : public GenericEntity
{
	Enemy(){
		CurrentTime = 0;
		CurrentAttackCount = 0;
		CurrentEnemyWave = 0;
	};
	~Enemy(){
		for (std::vector<EnemyProjectile*>::iterator it = cE_Projectiles.begin(); it != cE_Projectiles.end(); ++it)
		{
			delete *it;
		}
		cE_Projectiles.clear();
	};

	Mesh* EnemyMesh;
	std::string MeshName;

	int SpellPower;

	// Attacks in waves
	int CurrentAttackCount;
	float CurrentTime;

	// Monster Waves
	int MaxEnemyWave;
	int CurrentEnemyWave;

	std::vector<EnemyProjectile*> cE_Projectiles;
};

#endif //_ENEMY_H