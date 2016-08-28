#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Vector3.h"
#include "Mesh.h"
#include <string>
#include "GenericEntity.h"

class Billboard : public GenericEntity
{
public:
	Billboard();
	Billboard(const Vector3& Position, const Vector3& Dimensions, const Vector3& PlayerPosition, const Vector3& Velocity, const std::string& MeshName, const float& LifeTime = -1);
	virtual ~Billboard(){};
	
	bool Active = false;

	bool CheckLife();

	bool operator<(Billboard& rhs);

	virtual void Init(const Vector3& Position, const Vector3& Dimensions, const Vector3& PlayerPosition, const Vector3& Velocity, const std::string& MeshName, const float& LifeTime = -1);
	virtual void Update(float dt);
	virtual void Render();
	virtual void Exit(){};

	Vector3 GetPosition();
	Vector3 GetDimensions();
	Vector3 GetVelocity();
	std::string GetMeshName();
	float GetCurrTime();
	float GetLifeTime();

	void SetCurrentTime(float);
	void SetPosition(const Vector3& v3);
	void SetPlayerPosition(const Vector3&);
	void SetParameters(const Vector3& Position, const Vector3& Dimensions, const Vector3& PlayerPosition, const Vector3& Velocity, const std::string& MeshName, const float& CurrentTime, const float& LifeTime = -1);

	void SetDefaultLifeTime();

private:
	Vector3 Position;
	Vector3 Dimensions;
	Vector3 Velocity;
	std::string MeshName;
	Mesh* StoredMesh;
	Vector3 PlayerPosition;
	float LifeTime = 0;
	float CurrentTime = 0;

};

#endif