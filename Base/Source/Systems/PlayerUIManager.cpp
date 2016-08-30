#include "PlayerUIManager.h"
#include "Scene_System.h"

const std::string PlayerUIManager::UI_Text[10] = { "", "Statistics", "Inventory", "Quests", "Save Menu"};

PlayerUIManager::PlayerUIManager()
{
	HUD_Stats = nullptr;
	HUD_AddInfo = nullptr;
	HUD_Quests = nullptr;

	Menu_Base = nullptr;
	Menu_Stats = nullptr;
	Menu_Inventory = nullptr;
	Menu_Quests = nullptr;
	Menu_Save = nullptr;

	Tab_StatsButton = nullptr;
	Tab_InventoryButton = nullptr;
	Tab_QuestsButton = nullptr;
	Tab_SaveButton = nullptr;
}

PlayerUIManager::~PlayerUIManager()
{
	Exit();
}

void PlayerUIManager::Init()
{
	CenterPosition.Set(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	ButtonTimer = 0.f;
	WaitTime = 0.5f;

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
	// Menu Backing
	Menu_Base = new UI_Layer();
	Menu_Base->LayerOriginalPosition.y = 4.f * CenterPosition.y;
	Menu_Base->LayerCenterPosition.y = 4.f * CenterPosition.y;
	Menu_Base->LayerTargetPosition.y = 0;

	UI_Element* NewE = new UI_Element("Menu_Backing", CenterPosition, CenterPosition, Vector3(CenterPosition.x * 2.f, CenterPosition.y * 2.f, 1), CenterPosition);
	Menu_Base->cUI_Layer.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, "Statistics");
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.25f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, "Inventory");
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.75f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, "Quests"); 
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 1.25f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, "Save Menu");
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 1.75f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Base);

	// Menu Stats
	Menu_Stats = new UI_Layer();

	NewE = new UI_Element("TFB_Button", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f));
	Menu_Stats->cUI_Layer.push_back(NewE);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Stats);

	// Menu Save Screen
	Menu_Save = new UI_Layer();

	NewE = new UI_Element("TFB_Button", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f));
	Menu_Save->cUI_Layer.push_back(NewE);

    NewE = new UI_Element("TFB_Button", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), "Save Load 1");
    Menu_Save->cUI_Layer.push_back(NewE);
    NewE = new UI_Element("TFB_Button", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), "Save Load 2");
    Menu_Save->cUI_Layer.push_back(NewE);
    NewE = new UI_Element("TFB_Button", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), "Save Load 3");
    Menu_Save->cUI_Layer.push_back(NewE);
    NewE = new UI_Element("TFB_Button", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), "Quit Game");
    Menu_Save->cUI_Layer.push_back(NewE);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Save);
}

void PlayerUIManager::Update(double dt)
{
	ButtonTimer += (float)dt;
	if (ButtonTimer > WaitTime && Scene_System::accessing().cSS_InputManager->GetKeyValue('E'))
	{
		ButtonTimer = 0;
		if (CurrentState == UIS_HUD)
		{
			CurrentState = UIS_Menu_Stats;
		}
		else
		{
			if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
				Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
			CurrentState = UIS_HUD;
		}
	}

	if (CurrentState != UIS_HUD)
	{
		Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	}
	float DistMultiplier = 2.5f;
	switch (CurrentState)
	{
	case UIS_HUD:
		for (std::vector<UI_Layer*>::iterator it = UI_HUD.cUIS_LayerContainer.begin(); it != UI_HUD.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = 0;
		}
		for (std::vector<UI_Layer*>::iterator it = UI_Menu.cUIS_LayerContainer.begin(); it != UI_Menu.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = DistMultiplier * CenterPosition.y;
		}
		break;
	case UIS_Menu_Stats:
		for (std::vector<UI_Layer*>::iterator it = UI_HUD.cUIS_LayerContainer.begin(); it != UI_HUD.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = -DistMultiplier * CenterPosition.y;
		}
		for (std::vector<UI_Layer*>::iterator it = UI_Menu.cUIS_LayerContainer.begin(); it != UI_Menu.cUIS_LayerContainer.end(); ++it)
		{
			if ((*it) == Menu_Base || (*it) == Menu_Stats)
				(*it)->LayerTargetPosition.y = 0;
			else (*it)->LayerTargetPosition.y = DistMultiplier * CenterPosition.y;
		}
		break;
	case UIS_Menu_Inventory:
		for (std::vector<UI_Layer*>::iterator it = UI_HUD.cUIS_LayerContainer.begin(); it != UI_HUD.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = -DistMultiplier * CenterPosition.y;
		}
		for (std::vector<UI_Layer*>::iterator it = UI_Menu.cUIS_LayerContainer.begin(); it != UI_Menu.cUIS_LayerContainer.end(); ++it)
		{
			if ((*it) == Menu_Base || (*it) == Menu_Inventory)
				(*it)->LayerTargetPosition.y = 0;
			else (*it)->LayerTargetPosition.y = DistMultiplier * CenterPosition.y;
		}
		break;
	case UIS_Menu_Quests:
		for (std::vector<UI_Layer*>::iterator it = UI_HUD.cUIS_LayerContainer.begin(); it != UI_HUD.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = -DistMultiplier * CenterPosition.y;
		}
		for (std::vector<UI_Layer*>::iterator it = UI_Menu.cUIS_LayerContainer.begin(); it != UI_Menu.cUIS_LayerContainer.end(); ++it)
		{
			if ((*it) == Menu_Base || (*it) == Menu_Quests)
				(*it)->LayerTargetPosition.y = 0;
			else (*it)->LayerTargetPosition.y = DistMultiplier * CenterPosition.y;
		}
		break;
	case UIS_Menu_Save:
		for (std::vector<UI_Layer*>::iterator it = UI_HUD.cUIS_LayerContainer.begin(); it != UI_HUD.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = -DistMultiplier * CenterPosition.y;
		}
		for (std::vector<UI_Layer*>::iterator it = UI_Menu.cUIS_LayerContainer.begin(); it != UI_Menu.cUIS_LayerContainer.end(); ++it)
		{
			if ((*it) == Menu_Base || (*it) == Menu_Save)
				(*it)->LayerTargetPosition.y = 0;
			else (*it)->LayerTargetPosition.y = DistMultiplier * CenterPosition.y;
		}
		break;
	case UIS_NO_UI:
		for (std::vector<UI_Layer*>::iterator it = UI_HUD.cUIS_LayerContainer.begin(); it != UI_HUD.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = -DistMultiplier * CenterPosition.y;
		}
		for (std::vector<UI_Layer*>::iterator it = UI_Menu.cUIS_LayerContainer.begin(); it != UI_Menu.cUIS_LayerContainer.end(); ++it)
		{
			(*it)->LayerTargetPosition.y = DistMultiplier * CenterPosition.y;
		}
		break;
	}
	if (CurrentState != UIS_HUD)
	{
		for (std::vector<UI_Element*>::iterator it = Menu_BaseButtons.begin(); it != Menu_BaseButtons.end(); ++it)
		{
			(*it)->BoundsActive = true;
			bool CheckSucceeded = false;
			(*it)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), CheckSucceeded);
			if (CheckSucceeded)
			{
				if ((*it)->UI_Text == UI_Text[1])
				{
					// Statistics
					CurrentState = UIS_Menu_Stats;
				}
				else if ((*it)->UI_Text == UI_Text[2])
				{
					// Inventory
					CurrentState = UIS_Menu_Inventory;
				}
				else if ((*it)->UI_Text == UI_Text[3])
				{
					// Quests
					CurrentState = UIS_Menu_Quests;
				}
				else if ((*it)->UI_Text == UI_Text[4])
				{
					// Save Menu
					CurrentState = UIS_Menu_Save;
				}
				break;
			}
		}
	}
	UI_HUD.Update((float)dt);
	UI_Menu.Update((float)dt);
}

void PlayerUIManager::UpdateStats(float dt)
{
	UpdateStatsHUD((float)dt);
}

void PlayerUIManager::UpdateStatsHUD(float dt)
{
	// Name
	HUD_Stats->cUI_Layer[2]->UI_Text = Scene_System::accessing().gPlayer->getName();

	// SP
	std::stringstream ss;
	ss << "SP: " << Scene_System::accessing().gPlayer->GetSpellPower();
	HUD_Stats->cUI_Layer[3]->UI_Text = ss.str();

	// HP
	if (Scene_System::accessing().gPlayer->GetMaxHealth() < Scene_System::accessing().gPlayer->GetSpellPower())
		Scene_System::accessing().gPlayer->SetMaxHealth(Scene_System::accessing().gPlayer->GetMaxHealth());

	if (Scene_System::accessing().gPlayer->GetCurrentHealth() > Scene_System::accessing().gPlayer->GetMaxHealth())
		Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetMaxHealth());
	
	ss.str("");
	ss << "HP: " << Scene_System::accessing().gPlayer->GetCurrentHealth() << " / " << Scene_System::accessing().gPlayer->GetMaxHealth();
	HUD_Stats->cUI_Layer[4]->UI_Text = ss.str();
}

void PlayerUIManager::Render()
{
	UI_HUD.Render();
	UI_Menu.Render();
}

void PlayerUIManager::Exit()
{

}
