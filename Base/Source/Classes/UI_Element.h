#ifndef _UI_ELEMENT_H
#define _UI_ELEMENT_H

#include "GenericEntity.h"
#include "Vector3.h"
#include "Boundary2D.h"
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

	UI_Element(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text = "");

	virtual ~UI_Element();

	bool Active = false;
	Vector3 Position;
	Vector3 OriginalPosition;
	Vector3 TargetPosition;
	Vector3 Dimensions;
	UI_TYPES UI_Type = UI_UNASSIGNED;
	std::string UI_Text;
	bool AtTarget = false;
	bool BoundsActive = true;

	Boundary* UI_Bounds;

	void SwapOriginalWithTarget();

	virtual void Init(const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text);
	virtual void Update(float dt);
	virtual	void Update(float dt, const Vector3& MousePosition, bool& ClickSuccess);
	virtual void Render();
	virtual void Exit();

private:
	Mesh* StoredMesh = nullptr;
};



#endif //_UI_ELEMENT_H