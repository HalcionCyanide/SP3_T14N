#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Vector3.h"
#include "Mesh.h"

class Billboard{
public:
	Billboard(){};
	Billboard(Vector3 Position, Vector3 Dimensions, Vector3 PlayerPosition, Mesh* Mesh, float LifeTime = -1) :Position(Position), Dimensions(Dimensions), PlayerPosition(PlayerPosition), mesh(Mesh), LifeTime(LifeTime) {};
	Vector3 Position;
	Vector3 Dimensions;
	Vector3 Velocity;
	Mesh* mesh = nullptr;
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