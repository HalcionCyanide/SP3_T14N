#ifndef _UI_ELEMENT_H
#define _UI_ELEMENT_H

#include "GenericEntity.h"
#include "Vector3.h"
#include "Boundary.h"
#include "Mesh.h"
#include <string>

class UI_Element : public GenericEntity
{
public:
	enum UI_TYPES
	{
		UI_UNASSIGNED,
		UI_LOGO,
		UI_BUTTON_L_TO_SCRN,
		UI_BUTTON_T_TO_SCRN,
		UI_BUTTON_R_TO_SCRN,
		UI_BUTTON_B_TO_SCRN,
	};

	UI_Element(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& Dimensions, const Vector3& TargetPosition);
	virtual ~UI_Element();

	bool Active = false;
	Vector3 Position;
	Vector3 Dimensions;
	UI_TYPES UI_Type = UI_UNASSIGNED;

	virtual void Init(const std::string& name, const Vector3& Position, const Vector3& Dimensions, const Vector3& TargetPosition);
	virtual void Update(float dt);
	virtual void Render();
	virtual void Exit();

	Vector3 TargetPosition;

private:
	Mesh* StoredMesh = nullptr;
	Boundary UI_Bounds;
};



#endif //_UI_ELEMENT_H