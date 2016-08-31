#include "PlayerUIManager.h"
#include "Scene_System.h"
#include "..\Misc\SimpleCommand.h"
#include "../Mains/Application.h"

const std::string PlayerUIManager::UI_Text[10] = { "", "Statistics", "Inventory", "Quests", "Save Menu", "Save To Slot 1", "Save To Slot 2", "Save To Slot 3", "Exit To Main Menu", "Quit Game"};

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

	QuestLeftPointer = nullptr;
	QuestRightPointer = nullptr;
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
	QuestDisplayNumber = 0;
	CurrentQuestDisplayNumber = -1;

	int Counter = 0;
	for (std::vector<Quest*>::iterator it = Scene_System::accessing().QM.allQuests.begin(); it != Scene_System::accessing().QM.allQuests.end(); ++it)
	{
		ActiveQuestList.insert(std::pair<int, Quest*>(Counter, *it));
		++Counter;
	}

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

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, UI_Text[1]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.25f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, UI_Text[2]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.75f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, UI_Text[3]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 1.25f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 1), 0, UI_Text[4]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 1.75f, NewE->Dimensions.y*0.5f);
	Menu_Base->cUI_Layer.push_back(NewE);
	Menu_BaseButtons.push_back(NewE);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Base);

	//// Menu Stats
	Menu_Stats = new UI_Layer();

	// Backing
	NewE = new UI_Element("Menu_Backing2", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f));
	Menu_Stats->cUI_Layer.push_back(NewE);

	// Header
	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.4f, CenterPosition.y * 0.25f, 1), 0, UI_Text[1]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.32f, CenterPosition.y * 1.775f);
	Menu_Stats->cUI_Layer.push_back(NewE);

	// Name
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.25f, 1), 0, Scene_System::accessing().gPlayer->getName());
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x, CenterPosition.y * 1.6f);
	Menu_Stats->cUI_Layer.push_back(NewE);

	// HP
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x * 0.75f, CenterPosition.y * 0.25f, 1), 0, "HP");
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.55f, CenterPosition.y * 1.2f);
	Menu_Stats->cUI_Layer.push_back(NewE);

	// SP
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x * 0.75f, CenterPosition.y * 0.25f, 1), 0, "SP");
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 1.45f, CenterPosition.y * 1.2f);
	Menu_Stats->cUI_Layer.push_back(NewE);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Stats);

	//// Menu Quests
	Menu_Quests = new UI_Layer();

	// Backing
	NewE = new UI_Element("Menu_Backing2", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f));
	Menu_Quests->cUI_Layer.push_back(NewE);

	// Header
	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.4f, CenterPosition.y * 0.25f, 1), 0, UI_Text[3]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.32f, CenterPosition.y * 1.775f);
	Menu_Quests->cUI_Layer.push_back(NewE);

	// Title
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x * 0.75f, CenterPosition.y * 0.225f, 1), 0);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x, CenterPosition.y * 1.6f);
	Menu_Quests->cUI_Layer.push_back(NewE);
	Menu_QuestTextElements.push_back(NewE);

	// Description
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x * 1.2f, CenterPosition.y * 0.35f, 1), 0);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x, CenterPosition.y * 1.2f);
	Menu_Quests->cUI_Layer.push_back(NewE);
	Menu_QuestTextElements.push_back(NewE);

	// Rewards
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x * 1.2f, CenterPosition.y * 0.35f, 1), 0);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x, CenterPosition.y * 0.8f);
	Menu_Quests->cUI_Layer.push_back(NewE);
	Menu_QuestTextElements.push_back(NewE);

	// Giver
	NewE = new UI_Element("Menu_Backing3", 0, 0, Vector3(CenterPosition.x, CenterPosition.y * 0.175f, 1), 0);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x, CenterPosition.y * 0.455f);
	Menu_Quests->cUI_Layer.push_back(NewE);
	Menu_QuestTextElements.push_back(NewE);

	// Quest Pointers
	QuestLeftPointer = new UI_Element("Menu_LeftPointer", 0, 0, Vector3(CenterPosition.x * 0.2f, CenterPosition.y * 0.2f, 1), 0);
	QuestLeftPointer->Position = QuestLeftPointer->OriginalPosition = QuestLeftPointer->TargetPosition = Vector3(CenterPosition.x * 0.25f, CenterPosition.y);
	Menu_Quests->cUI_Layer.push_back(QuestLeftPointer);

	QuestRightPointer = new UI_Element("Menu_RightPointer", 0, 0, Vector3(CenterPosition.x * 0.2f, CenterPosition.y * 0.2f, 1), 0);
	QuestRightPointer->Position = QuestRightPointer->OriginalPosition = QuestRightPointer->TargetPosition = Vector3(CenterPosition.x * 1.75f, CenterPosition.y);
	Menu_Quests->cUI_Layer.push_back(QuestRightPointer);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Quests);

	//// Menu Save Screen
	Menu_Save = new UI_Layer();

	// Backing
	NewE = new UI_Element("Menu_Backing2", CenterPosition + Vector3(0, CenterPosition.y * 0.15f), CenterPosition + Vector3(0, CenterPosition.y * 0.15f), Vector3(CenterPosition.x * 1.9f, CenterPosition.y * 1.7f, 1), CenterPosition + Vector3(0, CenterPosition.y * 0.15f));
	Menu_Save->cUI_Layer.push_back(NewE);

	// Header
	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.4f, CenterPosition.y * 0.25f, 1), 0, UI_Text[4]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = Vector3(CenterPosition.x * 0.32f, CenterPosition.y * 1.775f);
	Menu_Save->cUI_Layer.push_back(NewE);

	// Buttons
    NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.2f, 1), 0, UI_Text[5]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = CenterPosition + Vector3(-CenterPosition.x * 0.4f, CenterPosition.y * 0.35f);
	Menu_Save->cUI_Layer.push_back(NewE);	
	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.2f, 1), 0, UI_Text[6]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = CenterPosition + Vector3(-CenterPosition.x * 0.4f);
	Menu_Save->cUI_Layer.push_back(NewE);
	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.2f, 1), 0, UI_Text[7]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = CenterPosition + Vector3(-CenterPosition.x * 0.4f, CenterPosition.y * -0.35f);
	Menu_Save->cUI_Layer.push_back(NewE);

	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.2f, 1), 0, UI_Text[8]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = CenterPosition + Vector3(CenterPosition.x * 0.4f, CenterPosition.y * 0.2f);
	Menu_Save->cUI_Layer.push_back(NewE);
	NewE = new UI_Element("TFB_Button", 0, 0, Vector3(CenterPosition.x * 0.6f, CenterPosition.y * 0.2f, 1), 0, UI_Text[9]);
	NewE->Position = NewE->OriginalPosition = NewE->TargetPosition = CenterPosition + Vector3(CenterPosition.x * 0.4f, CenterPosition.y * -0.2f);
	Menu_Save->cUI_Layer.push_back(NewE);

	UI_Menu.cUIS_LayerContainer.push_back(Menu_Save);
}

void PlayerUIManager::Update(double dt)
{
	bool CheckSucceeded;
	ButtonTimer += (float)dt;
	if (CurrentState != UIS_NO_UI && ButtonTimer > WaitTime && Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::PAUSE_MENU_COMMAND]))
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
			Scene_System::accessing().cSS_InputManager->SetMouseToScreenCenter();
		}
	}

	if (CurrentState != UIS_HUD)
	{
		Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	}
	float DistMultiplier = 2.5f;
	std::stringstream ss;

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
		UpdateStatsHUD((float)dt);
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
		ss << "HP: " << Scene_System::accessing().gPlayer->GetCurrentHealth() << " / " << Scene_System::accessing().gPlayer->GetMaxHealth();
		Menu_Stats->cUI_Layer[3]->UI_Text = ss.str();
		ss.str("");
		ss << "SP: " << Scene_System::accessing().gPlayer->GetSpellPower();
		Menu_Stats->cUI_Layer[4]->UI_Text = ss.str();
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
		CheckSucceeded = false;
		QuestLeftPointer->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), CheckSucceeded);
		if (ButtonTimer > WaitTime && CheckSucceeded) {
			ButtonTimer = 0;
			QuestDisplayNumber++;
		}
		CheckSucceeded = false;
		QuestRightPointer->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), CheckSucceeded);
		if (ButtonTimer > WaitTime && CheckSucceeded) {
			ButtonTimer = 0;
			QuestDisplayNumber++;
		}
		if (CurrentQuestDisplayNumber != QuestDisplayNumber)
		{
			if (CurrentQuestDisplayNumber < 0)
				CurrentQuestDisplayNumber = ActiveQuestList.size();
			CurrentQuestDisplayNumber = QuestDisplayNumber;
			UpdateQuestsMenu((float)dt);
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
            //Updating of the Keys
            if ((*it) == Menu_Save) {
                for (std::vector<UI_Element*>::iterator it2 = (*it)->cUI_Layer.begin(), end2 = (*it)->cUI_Layer.end(); it2 != end2; ++it2)
                {
                    if ((*it2)->UI_Text == "")
                        continue;
                    (*it2)->BoundsActive = true;
                    bool CheckSucceeded = false;
                    (*it2)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), CheckSucceeded);
                    if (CheckSucceeded) {
                        if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
                            Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
						if ((*it2)->UI_Text == UI_Text[5])
                        {
                            Scene_System::accessing().gPlayer->settingTheFileToSave(1);
                            Scene_System::accessing().gPlayer->automaticallySaveFile();
                        }
						else if ((*it2)->UI_Text == UI_Text[6])
                        {
                            Scene_System::accessing().gPlayer->settingTheFileToSave(2);
                            Scene_System::accessing().gPlayer->automaticallySaveFile();
                        }
						else if ((*it2)->UI_Text == UI_Text[7])
                        {
                            Scene_System::accessing().gPlayer->settingTheFileToSave(3);
                            Scene_System::accessing().gPlayer->automaticallySaveFile();
                        }
						else if ((*it2)->UI_Text == UI_Text[8])
                        {
                            Scene_System::accessing().getCurrScene().onNotify("LOADING_M_Scene");
                            Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
                        }
						else if ((*it2)->UI_Text == UI_Text[9])
						{
							Application::ExitGame = true;
						}
                        CurrentState = UIS_HUD;
                        Scene_System::accessing().cSS_InputManager->SetMouseToScreenCenter();
                        break;
                    }
                }
            }
            //Updating of the Keys
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
	std::stringstream ss;

	// Open Menu
	ss << "<" << SimpleCommand::m_allTheKeys[SimpleCommand::PAUSE_MENU_COMMAND] << "> - Menu | " << "<" << SimpleCommand::m_allTheKeys[SimpleCommand::INTERACT_COMMAND] << "> - Interact";
	HUD_Stats->cUI_Layer[1]->UI_Text = ss.str();

	// Name
	HUD_Stats->cUI_Layer[2]->UI_Text = Scene_System::accessing().gPlayer->getName();

	// SP
	ss.str("");
	ss << "SP: " << Scene_System::accessing().gPlayer->GetSpellPower();
	HUD_Stats->cUI_Layer[3]->UI_Text = ss.str();

	// HP
	if (Scene_System::accessing().gPlayer->GetMaxHealth() > Scene_System::accessing().gPlayer->GetSpellPower())
		Scene_System::accessing().gPlayer->SetMaxHealth(Scene_System::accessing().gPlayer->GetSpellPower());

	if (Scene_System::accessing().gPlayer->GetCurrentHealth() > Scene_System::accessing().gPlayer->GetMaxHealth())
		Scene_System::accessing().gPlayer->SetCurrentHealth(Scene_System::accessing().gPlayer->GetMaxHealth());
	
	ss.str("");
	ss << "HP: " << Scene_System::accessing().gPlayer->GetCurrentHealth() << " / " << Scene_System::accessing().gPlayer->GetMaxHealth();
	HUD_Stats->cUI_Layer[4]->UI_Text = ss.str();
}

void PlayerUIManager::UpdateQuestsMenu(float dt)
{
	std::map<std::string, int> PlayerStates = Scene_System::accessing().gPlayer->playerCurrQState;
	if (QuestDisplayNumber > (int)PlayerStates.size() - 1)
	{
		QuestDisplayNumber = 0;
	}
	std::map<int, Quest*>::iterator Quest = ActiveQuestList.find(QuestDisplayNumber);
	if (Quest->second != nullptr)
	{
		CurrentQuest = Quest->second;
		bool Check = false;
		for (std::vector<QuestStage*>::iterator it = CurrentQuest->qStages.begin(); it != CurrentQuest->qStages.end(); ++it)
		{
			std::map<std::string, int>::iterator StateCheck = PlayerStates.find(CurrentQuest->getName());
			if (Quest->second->getCurrentStage() > 0)
			{
				if (!(*it)->getComplete() && CurrentQuest->getActive() && (*it)->getStageNO() < StateCheck->second)
				{
					CurrentStage = *it;
					Menu_QuestTextElements[0]->UI_Text = Quest->second->getName();
					Menu_QuestTextElements[1]->UI_Text = (*it)->getDesc();
					Menu_QuestTextElements[1]->WrapText();
					Menu_QuestTextElements[3]->UI_Text = "Given By: " + (*it)->getGiver();
					Check = true;
					break;
				}
			}
			else
			{
				Menu_QuestTextElements[0]->UI_Text = "< No Quests >";
				Menu_QuestTextElements[1]->UI_Text = "Description: -";
				Menu_QuestTextElements[2]->UI_Text = "Reward: -";
				Menu_QuestTextElements[3]->UI_Text = "Given By: -";
			}
		}
		if (!Check)
		{
			++QuestDisplayNumber;
			if (QuestDisplayNumber > (int)PlayerStates.size() -1)
			{
				QuestDisplayNumber = 0;
			}
		}
	}
}

void PlayerUIManager::Render()
{
	UI_HUD.Render();
	UI_Menu.Render();
}

void PlayerUIManager::Exit()
{

}
