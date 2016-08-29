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
	UI_Element* NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.25f, 1), 0);
	NewE->TargetPosition.Set(NewE->Dimensions.x * 0.5f, NewE->Dimensions.y * 0.5f);
	NewE->UI_Text_Container.push_back("My HP");
	NewE->UI_Text_Container.push_back("My SP");
	NewE->TextWrappingEnabled = true;
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
