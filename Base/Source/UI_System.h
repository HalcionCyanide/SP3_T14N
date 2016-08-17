#ifndef _UI_SYSTEM_H
#define _UI_SYSTEM_H

#include "GenericSystem.h"
#include "Vector3.h"
#include "UI_Element.h"

#include <vector>

class UI_System : public GenericSystem
{
public:
	UI_System();
	virtual ~UI_System();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void AddUIElement(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& Dimension, const Vector3& TargetPosition);

private:
	std::vector<UI_Element*> cUIS_ElementContainer;

};

#endif // _UI_SYSTEM_H