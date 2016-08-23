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
	for (std::vector<UI_Layer*>::iterator it = cUIS_LayerContainer.begin(); it != cUIS_LayerContainer.end(); ++it)
	{
		if ((*it)->cUI_Layer.size() > 0)
			(*it)->Update((float)dt);
	}
}

void UI_System::Render()
{
	for (std::vector<UI_Layer*>::iterator it = cUIS_LayerContainer.begin(); it != cUIS_LayerContainer.end(); ++it)
	{
		if ((*it)->cUI_Layer.size() > 0)
			(*it)->Render();
	}
}

void UI_System::Exit()
{
	for (std::vector<UI_Layer*>::iterator it = cUIS_LayerContainer.begin(); it != cUIS_LayerContainer.end(); ++it)
    {
        (*it)->Exit();
        delete *it;
    }
	cUIS_LayerContainer.clear();
}