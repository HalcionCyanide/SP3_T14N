#include "Boundary.h"

Boundary::Boundary()
{}

Boundary::~Boundary()
{}

bool Boundary::CheckCollision(GameObject* object, Camera* player)
{
	Vector3 ObjectMin, ObjectMax, PlayerMin, PlayerMax;
	Vector3 CameraScale = Vector3(3.f, 10.f, 3.f);
	ObjectMax.Set(object->GetPos().x + (object->GetScale().x * 0.5), object->GetPos().y + (object->GetScale().y * 0.5), object->GetPos().z + (object->GetScale().z * 0.5));
	ObjectMin.Set(object->GetPos().x - (object->GetScale().x * 0.5), object->GetPos().y - (object->GetScale().y * 0.5), object->GetPos().z - (object->GetScale().z * 0.5));
	PlayerMax.Set(player->position.x + (CameraScale.x * 0.5), player->position.y + (CameraScale.y * 0.5), player->position.z + (CameraScale.z * 0.5));
	PlayerMin.Set(player->position.x - (CameraScale.x * 0.5), player->position.y - (CameraScale.y * 0.5), player->position.z - (CameraScale.z * 0.5));

	return ((ObjectMax.x > PlayerMin.x || PlayerMax.x > ObjectMin.x) && (ObjectMax.y > PlayerMin.y || PlayerMax.y > ObjectMin.y) && (ObjectMax.z > PlayerMin.z || PlayerMax.z > ObjectMin.z));
}