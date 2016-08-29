#include "PlayerUIManager.h"
#include "Scene_System.h"

PlayerUIManager::PlayerUIManager()
{
	HUD_Stats= nullptr;
	HUD_AddInfo= nullptr;
	HUD_Quests= nullptr;
	
	Menu_Stats= nullptr;
	Menu_Inventory= nullptr;
	Menu_Quests= nullptr; 
	Menu_Save = nullptr;
	
	Tab_StatsButton= nullptr;
	Tab_InventoryButton= nullptr;
	Tab_QuestsButton= nullptr;
	Tab_SaveButton= nullptr;
}

PlayerUIManager::~PlayerUIManager()
{
	Exit();
}

void PlayerUIManager::Init()
{
	CenterPosition.Set(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	UI_HUD.Init();
	UI_Menu.Init();
	InitHUD();
	InitMenu();
}

void PlayerUIManager::InitHUD()
{
	HUD_Stats = new UI_Layer();

	HUD_Stats->LayerTargetPosition.y = 0;
	HUD_Stats->LayerCenterPosition.y = 0;
	HUD_Stats->LayerOriginalPosition.y = -CenterPosition.y;

	UI_Element* NewE = new UI_Element("HUD_Stats", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.5f, 1), 0);
	NewE->TargetPosition.Set(NewE->Dimensions.x * 0.5f, NewE->Dimensions.y * 0.5f);
	NewE->Position = NewE->TargetPosition;
	HUD_Stats->cUI_Layer.push_back(NewE);

	NewE = new UI_Element("TextBacking", 0, 0, Vector3(CenterPosition.x * 0.35f, CenterPosition.y * 0.11f, 1), 0, "Open Menu - <Put Key Here>");
	NewE->TargetPosition.Set(NewE->Dimensions.x * 0.625f, CenterPosition.y * 0.465f);
	NewE->TextColor.SetZero();
	NewE->Position = NewE->TargetPosition;
	HUD_Stats->cUI_Layer.push_back(NewE);

	NewE = new UI_Element("UI_ChatBox", 0, 0, Vector3(CenterPosition.x * 0.35f, CenterPosition.y * 0.11f, 1), 0, Scene_System::accessing().gPlayer->getName());
	NewE->TargetPosition.Set(NewE->Dimensions.x * 0.65f, CenterPosition.y * 0.375f);
	NewE->Position = NewE->TargetPosition;
	HUD_Stats->cUI_Layer.push_back(NewE);

	std::stringstream ss;
	ss << "SP: " << Scene_System::accessing().gPlayer->GetSpellPower();
	NewE = new UI_Element("UI_ChatBox", 0, 0, Vector3(CenterPosition.x * 0.35f, CenterPosition.y * 0.11f, 1), 0, ss.str());
	NewE->TargetPosition.Set(NewE->Dimensions.x * 0.65f, CenterPosition.y * 0.245f);
	NewE->Position = NewE->TargetPosition;
	HUD_Stats->cUI_Layer.push_back(NewE);

	ss.str("");
	ss << "HP: " << Scene_System::accessing().gPlayer->GetCurrentHealth() << " / " << Scene_System::accessing().gPlayer->GetMaxHealth();
	NewE = new UI_Element("UI_ChatBox", 0, 0, Vector3(CenterPosition.x * 0.35f, CenterPosition.y * 0.11f, 1), 0, ss.str());
	NewE->TargetPosition.Set(NewE->Dimensions.x * 0.65f, CenterPosition.y  * 0.115f);
	NewE->Position = NewE->TargetPosition;
	HUD_Stats->cUI_Layer.push_back(NewE);

	UI_HUD.cUIS_LayerContainer.push_back(HUD_Stats);
}

void PlayerUIManager::InitMenu()
{

}

void PlayerUIManager::Update(double dt)
{
	UI_HUD.Update((float)dt);
	UI_Menu.Update((float)dt);
}

void PlayerUIManager::Render()
{
	UI_HUD.Render();
}

void PlayerUIManager::Exit()
{

}
