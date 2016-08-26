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

	Enemy(Enemy& E)
	{
		//EnemyMesh = E.EnemyMesh;
		//MeshName = E.MeshName;
		//SpellPower = E.SpellPower;
		//MaxEnemyWave = E.MaxEnemyWave;
		//CurrentAttackCount = 0;
		//CurrentEnemyWave = 0;
		//CurrentTime = 0;
		*this = E;
	}
	Enemy &operator=(Enemy &rhs)
	{
		EnemyMesh = rhs.EnemyMesh;
		MeshName = rhs.MeshName;
		SpellPower = rhs.SpellPower;
		MaxEnemyWave = rhs.MaxEnemyWave;
		CurrentAttackCount = 0;
		CurrentEnemyWave = 0;
		CurrentTime = 0;
		for (std::vector<EnemyProjectile*>::iterator it = rhs.cE_Projectiles.begin(), end = rhs.cE_Projectiles.end(); it != end; ++it)
		{
			EnemyProjectile *zeProjectile = new EnemyProjectile(**it);
			cE_Projectiles.push_back(zeProjectile);
		}
		return *this;
	}

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