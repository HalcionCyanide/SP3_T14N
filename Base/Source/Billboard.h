#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Vector3.h"
#include <string>

class Billboard{
public:
	Billboard(){};
	Billboard(Vector3 Position, Vector3 Dimensions, Vector3 PlayerPosition, std::string MeshName, float LifeTime = -1) :Position(Position), Dimensions(Dimensions), PlayerPosition(PlayerPosition), MeshName(MeshName), LifeTime(LifeTime) {};
	~Billboard(){};
	
	bool CheckLife();
	bool operator<(Billboard& rhs);

	Vector3 GetPosition();
	Vector3 GetDimensions();
	Vector3 GetVelocity();
	std::string GetMeshName();
	float GetCurrTime();
	float GetLifeTime();

	void SetCurrentTime(float);
	void SetPosition(const Vector3& v3);
	void SetPlayerPosition(const Vector3&);
	void SetParameters(Vector3 Position, Vector3 Dimensions, Vector3 PlayerPosition, Vector3 Velocity, std::string MeshName, float CurrentTime, float LifeTime);

	bool Active = false;

private:
	Vector3 Position;
	Vector3 Dimensions;
	Vector3 Velocity;
	std::string MeshName;
	Vector3 PlayerPosition;
	float LifeTime = 0;
	float CurrentTime = 0;

};

#endif