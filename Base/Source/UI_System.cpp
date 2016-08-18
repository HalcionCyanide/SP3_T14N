#include "UI_System.h"

UI_System::UI_System()
{
	Init();
}

UI_System::~UI_System()
{
	Exit();
}

void UI_System::Init()
{

}

void UI_System::Update(double dt)
{
	for (std::vector<UI_Element*>::iterator it = cUIS_ElementContainer.begin(); it != cUIS_ElementContainer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Update((float)dt);
	}
}

void UI_System::Render()
{
	for (std::vector<UI_Element*>::iterator it = cUIS_ElementContainer.begin(); it != cUIS_ElementContainer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Render();
	}
}

void UI_System::Exit()
{
	for (std::vector<UI_Element*>::iterator it = cUIS_ElementContainer.begin(); it != cUIS_ElementContainer.end(); ++it)
		(*it)->Exit();
}

void UI_System::AddUIElement(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimension, const Vector3& TargetPosition, const std::string& UI_Text)
{
	cUIS_ElementContainer.push_back(new UI_Element(UI_Type, name, Position, SpawnPosition, Dimension, TargetPosition, UI_Text));
}