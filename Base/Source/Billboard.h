#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Vector3.h"
#include <string>

class Billboard{
public:
	Billboard(){};
	Billboard(Vector3 Position, Vector3 Dimensions, Vector3 PlayerPosition, std::string MeshName, float LifeTime = -1) :Position(Position), Dimensions(Dimensions), PlayerPosition(PlayerPosition), MeshName(MeshName), LifeTime(LifeTime) {};
	Vector3 Position;
	Vector3 Dimensions;
	Vector3 Velocity;
	std::string MeshName;
	Vector3 PlayerPosition;
	float LifeTime = 0;
	float CurrentTime = 0;
	bool IsParticle = true;
	bool Active = false;

	bool CheckLife();

	void SetType(bool IsParticle);

	bool operator<(Billboard& rhs);

};

#endif