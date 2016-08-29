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
	UI_Element(const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text = "");

	virtual ~UI_Element();

	bool Active = false;
	Vector3 Position;
	Vector3 OriginalPosition;
	Vector3 TargetPosition;
	Vector3 Dimensions;
	std::string UI_Text;
	Vector3 TextColor;
	bool AtTarget = false;
	bool BoundsActive = true;

	std::vector<std::string> UI_Text_Container;

	void WrapText();
	bool TextWrappingEnabled = false;

	Boundary2D* UI_Bounds;

	void SwapOriginalWithTarget();

	virtual void Init(const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text);
	virtual void Update(float dt);
	virtual	void Update(float dt, const Vector3& MousePosition, bool& ClickSuccess);
	virtual void Render(const Vector3& LayerPos);
	virtual void Exit();

private:
	Mesh* StoredMesh = nullptr;
};



#endif //_UI_ELEMENT_H