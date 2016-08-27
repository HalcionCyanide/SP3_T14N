#include "UI_Element.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

UI_Element::UI_Element(const UI_Element::UI_TYPES& UI_Type, const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimensions, const Vector3& TargetPosition, const std::string& UI_Text)
{
	this->UI_Type = UI_Type;
	this->UI_Bounds = nullptr;
	Init(name, Position, SpawnPosition, Dimensions, TargetPosition, UI_Text);
}

UI_Element::~UI_Element()
{
	if (UI_Bounds)
	{
		delete UI_Bounds;
		UI_Bounds = nullptr;
	}
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
		UI_Bounds->SetPosition(Position);
		UI_Bounds->SetDimensions(Dimensions);
	}
}

void UI_Element::SwapOriginalWithTarget()
{
	std::swap(OriginalPosition, TargetPosition);
}

void UI_Element::WrapText()
{
	UI_Text_Container.clear();
	float SingleSpace = 0.75f * Dimensions.y * 0.3f;
	float RequiredSpace = UI_Text.size() * SingleSpace;
	int MaxCharactersPerLine = (int)(Dimensions.x / SingleSpace);
	std::stringstream Tmp, MainString;
	int CPos = 0;
	int CCheck = 0;
	if (MaxCharactersPerLine < (int)UI_Text.size())
		while (CPos < (int)UI_Text.size())
		{
			Tmp << UI_Text[CPos];
			++CPos;
			if (CPos == UI_Text.size() || UI_Text[CPos] == ' ')
			{
				MainString << Tmp.str();
				Tmp.str("");
			}
			if (CPos == UI_Text.size() || CPos >= MaxCharactersPerLine + MaxCharactersPerLine * CCheck)
			{
				CCheck++;
				UI_Text_Container.push_back(MainString.str());
				MainString.str("");
				if (Tmp)
				{
					MainString << Tmp.str();
				}
				Tmp.str("");
				TextWrappingEnabled = true;
			}
		}
	else TextWrappingEnabled = false;
}

void UI_Element::Update(float dt)
{
	float Check = (TargetPosition - Position).LengthSquared();
	if (!AtTarget && Active && Check > 1.f)
	{
		UI_Bounds->SetPosition(Position);
		UI_Bounds->SetDimensions(Dimensions);
		UI_Bounds->ResetValues();
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
		UI_Bounds->SetPosition(Position);
		UI_Bounds->SetDimensions(Dimensions);
		UI_Bounds->ResetValues();
		Vector3 DirVec = TargetPosition - Position;
		Position += DirVec * 3 * dt;
		if (Check < 1.f)
			AtTarget = true;
	}
	if (Active)
	{
		if (BoundsActive && UI_Bounds->CheckCollision(MousePosition))
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

void UI_Element::Render(const Vector3& LayerPos)
{
	if (Active)
	{
		glDisable(GL_CULL_FACE);
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		SceneGraphics->RenderMeshIn2D(*StoredMesh, false, Dimensions.x, Dimensions.y, Position.x + LayerPos.x, Position.y + LayerPos.y);
		if (!TextWrappingEnabled)
			SceneGraphics->RenderTextOnScreen(UI_Text, Color(1, 1, 1), Dimensions.y * 0.3f, Position.x + LayerPos.x - (UI_Text.size() * 0.5f * 0.75f * Dimensions.y * 0.3f), Position.y + LayerPos.y - (0.5f * Dimensions.y * 0.3f));
		else if (TextWrappingEnabled)
			for (unsigned int i = 0; i < UI_Text_Container.size(); ++i)
			{
				SceneGraphics->RenderTextOnScreen(UI_Text_Container[i], Color(1, 1, 1), Dimensions.y * 0.25f, Position.x + LayerPos.y - Dimensions.x * 0.5f + (Dimensions.y * 0.25f), Position.y + LayerPos.y + (0.5f * Dimensions.y * 0.25f) - (i * Dimensions.y * 0.25f));
			}
		glEnable(GL_CULL_FACE);
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