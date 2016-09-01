#include "..\\Classes\\UI_Layer.h"
#include <algorithm>

UI_Layer::UI_Layer()
{
	Init();
}

UI_Layer::~UI_Layer()
{
	Exit();
}

void UI_Layer::Init()
{
	LayerCenterPosition = Vector3();
	LayerTargetPosition = Vector3();
}

void UI_Layer::Update(double dt)
{
	Vector3 DirVec = LayerTargetPosition - LayerCenterPosition;
	float Check = DirVec.LengthSquared();
	if (Check > 1.f)
	{
		LayerCenterPosition += DirVec * 3 * (float)dt;
	}

	for (std::vector<UI_Element*>::iterator it = cUI_Layer.begin(); it != cUI_Layer.end(); ++it)
	{
		if ((*it)->Active)
		{
            (*it)->UI_Bounds->SetPosition((*it)->Position + LayerCenterPosition);
            (*it)->UI_Bounds->SetDimensions((*it)->Dimensions);
            (*it)->UI_Bounds->ResetValues();
			(*it)->Update((float)dt);
		}
	}
}

void UI_Layer::Render()
{
	for (std::vector<UI_Element*>::iterator it = cUI_Layer.begin(); it != cUI_Layer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Render(LayerCenterPosition);
	}
}

void UI_Layer::Exit()
{
	for (std::vector<UI_Element*>::iterator it = cUI_Layer.begin(); it != cUI_Layer.end(); ++it)
	{
		(*it)->Exit();
		delete *it;
	}
	cUI_Layer.clear();
}

void UI_Layer::AddUIElement(const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimension, const Vector3& TargetPosition, const std::string& UI_Text)
{
	cUI_Layer.push_back(new UI_Element(name, Position, SpawnPosition, Dimension, TargetPosition, UI_Text));
}

void UI_Layer::SwapOriginalWithTarget()
{
	std::swap(LayerOriginalPosition, LayerTargetPosition);
}