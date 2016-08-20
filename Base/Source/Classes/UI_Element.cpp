#include "UI_Element.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

UI_Element::UI_Element(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text)
{
	this->UI_Type = UI_Type;
	Init(name, Position, SpawnPosition, Dimensions, TargetPosition, UI_Text);
}

UI_Element::~UI_Element()
{
	Exit();
}

void UI_Element::Init(const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(name);
	if (it != SceneGraphics->meshList.end())
	{
		StoredMesh = it->second;
		Active = true;
		this->Position = SpawnPosition;
		this->OriginalPosition = Position;
		this->Dimensions = Dimensions;
		this->TargetPosition = TargetPosition;
		this->Dimensions = Dimensions;
		this->UI_Text = UI_Text;
		UI_Bounds = new Boundary2D();
		UI_Bounds->CalculateValues(Position, Dimensions);
	}
}

void UI_Element::SwapOriginalWithTarget()
{
	std::swap(OriginalPosition, TargetPosition);
}

void UI_Element::Update(float dt)
{
	float Check = (TargetPosition - Position).LengthSquared();
	if (!AtTarget && Active && Check > 1.f)
	{
		Vector3 DirVec = TargetPosition - Position;
		Position += DirVec * 2 * dt;
		if (Check < 1.f)
			AtTarget = true;
	}
}

void UI_Element::Update(float dt, const Vector3& MousePosition, bool& ClickSuccess)
{
	float Check = (TargetPosition - Position).LengthSquared();
	if (!AtTarget && Active && Check > 1.f)
	{
		UI_Bounds->CalculateValues(Position, Dimensions);
		Vector3 DirVec = TargetPosition - Position;
		Position += DirVec * 3 * dt;
		if (Check < 1.f)
			AtTarget = true;
	}
	if (Active)
	{
		if (BoundsActive && UI_Bounds->CheckCollision(MousePosition, Vector3(0, 0, 0)))
		{
			// Do Upscale
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				//BoundsActive = false;
				ClickSuccess = true;
			}
		}
	}
}

void UI_Element::Render()
{
	if (Active)
	{
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		SceneGraphics->RenderMeshIn2D(*StoredMesh, false, Dimensions.x, Dimensions.y, Position.x, Position.y);
		SceneGraphics->RenderTextOnScreen(UI_Text, Color(1, 1, 1), Dimensions.y * 0.3f, Position.x - (UI_Text.size() * 0.5f * 0.75f * Dimensions.y * 0.3f), Position.y - (0.5f * Dimensions.y * 0.3f));
	}
}

void UI_Element::Exit()
{
	if (UI_Bounds)
	{
		delete UI_Bounds;
		UI_Bounds = nullptr;
	}
}