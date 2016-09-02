/****************************************************************************/
/*!
\file Enemy.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines an Enemy
*/
/****************************************************************************/

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
		*this = E;
	}
	Enemy &operator=(Enemy &rhs)
	{
		EnemyName = rhs.EnemyName;
		EnemyType = rhs.EnemyType;
		EnemyMesh = rhs.EnemyMesh;
		MeshName = rhs.MeshName;
		SpellPower = rhs.SpellPower;
		MaxEnemyWave = rhs.MaxEnemyWave;
		CurrentAttackCount = 0;
		CurrentEnemyWave = 0;
		CurrentTime = 0;
		for (std::vector<EnemyProjectile*>::iterator it = rhs.cE_Projectiles.begin(), end = rhs.cE_Projectiles.end(); it != end; ++it)
		{
			EnemyProjectile *Projectile = new EnemyProjectile(**it);
			cE_Projectiles.push_back(Projectile);
		}
		return *this;
	}

	Mesh* EnemyMesh;
	std::string MeshName;
	std::string EnemyName;
	std::string EnemyType;

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