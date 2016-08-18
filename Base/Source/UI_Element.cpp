#include "UI_Element.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"

UI_Element::UI_Element(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& Dimensions, const Vector3& TargetPosition)
{
	this->UI_Type = UI_Type;
	Init(name, Position, Dimensions, TargetPosition);
}

UI_Element::~UI_Element()
{
	Exit();
}

void UI_Element::Init(const std::string& name, const Vector3& Position, const Vector3& Dimensions, const Vector3& TargetPosition)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(name);
	if (it != SceneGraphics->meshList.end())
	{
		StoredMesh = it->second;
		Active = true;
		this->Position = this->OriginalPosition = Position;
		this->Dimensions = Dimensions;
		this->TargetPosition = TargetPosition;
		this->Dimensions = Dimensions;
	}
}

void UI_Element::SwapOriginalWithTarget()
{
	std::swap(OriginalPosition, TargetPosition);
}

void UI_Element::Update(float dt)
{
	if (Active && (TargetPosition - Position).LengthSquared() > 1.f)
	{
		Vector3 DirVec = TargetPosition - Position;
		Position += DirVec * 2 * dt;
	}
	else if (Active)SwapOriginalWithTarget();
}

void UI_Element::Render()
{
	if (Active)
	{
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		SceneGraphics->RenderMeshIn2D(*StoredMesh, false, Dimensions.x, Dimensions.y, Position.x, Position.y);
	}
}

void UI_Element::Exit()
{
	/*if (StoredMesh)
		delete StoredMesh;*/
}