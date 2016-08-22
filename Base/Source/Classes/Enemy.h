#ifndef _ENEMY_H
#define _ENEMY_H

// Stats Container for an Enemy Object
struct Enemy
{
	Enemy(){};
	~Enemy(){};

	int SpellPower = 1;
	float AttackSpeed = 1;
	int AttacksPerWave = 1;
	int CurrentAttackCount = 0;
	float CurrentTime = 0;
	float DamagePerAttack = 1;
	int MaxEnemyWave = 10;
	int CurrentEnemyWave = 0;


};

#endif //_ENEMY_H