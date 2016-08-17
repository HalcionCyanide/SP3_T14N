#include "Boundary.h"

Boundary::Boundary()
{}

Boundary::~Boundary()
{}

bool Boundary::CheckCollision(GameObject* object, GameObject* player)
{
	Vector3 ObjectMin, ObjectMax, PlayerMin, PlayerMax;
	ObjectMax.Set(float(object->GetPos().x + (object->GetScale().x * 0.5f)), float(object->GetPos().y + (object->GetScale().y * 0.5f)), float(object->GetPos().z + (object->GetScale().z * 0.5f)));
	ObjectMin.Set(float(object->GetPos().x - (object->GetScale().x * 0.5f)), float(object->GetPos().y - (object->GetScale().y * 0.5f)), float(object->GetPos().z - (object->GetScale().z * 0.5f)));
	PlayerMax.Set(float(player->GetPos().x + (player->GetScale().x * 0.5f)), float(player->GetPos().y + (player->GetScale().y * 0.5f)), float(player->GetPos().z + (player->GetScale().z * 0.5f)));
	PlayerMin.Set(float(player->GetPos().x - (player->GetScale().x * 0.5f)), float(player->GetPos().y - (player->GetScale().y * 0.5f)), float(player->GetPos().z - (player->GetScale().z * 0.5f)));

	return ((ObjectMax.x > PlayerMin.x || PlayerMax.x > ObjectMin.x) && (ObjectMax.y > PlayerMin.y || PlayerMax.y > ObjectMin.y) && (ObjectMax.z > PlayerMin.z || PlayerMax.z > ObjectMin.z));
}

bool Boundary::CheckCollision2D(UI_Element* object, Vector3 point)
{
	Vector3 ObjectMin, ObjectMax;
	ObjectMax.Set(float(object->Position.x + (object->Dimensions.x * 0.5f)), float(object->Position.y + (object->Dimensions.y * 0.5f)), float(object->Position.z + (object->Dimensions.z * 0.5f)));
	ObjectMin.Set(float(object->Position.x - (object->Dimensions.x * 0.5f)), float(object->Position.y - (object->Dimensions.y * 0.5f)), float(object->Position.z - (object->Dimensions.z * 0.5f)));

	return ((ObjectMax.x > point.x || point.x > ObjectMin.x) && (ObjectMax.y > point.y || point.y > ObjectMin.y));
}