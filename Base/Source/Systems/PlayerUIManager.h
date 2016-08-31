#ifndef _PLAYER_UI_MANAGER_H
#define _PLAYER_UI_MANAGER_H

#include "UI_System.h"
#include "../Quests/Quest.h"
#include <map>

class PlayerUIManager : public GenericSystem
{
public:
	enum UI_State
	{
		UIS_HUD,
		UIS_Menu_Stats,
		UIS_Menu_Inventory,
		UIS_Menu_Quests,
		UIS_Menu_Save,
		UIS_NO_UI,
	};

	UI_State CurrentState = UIS_HUD;

	PlayerUIManager();
	~PlayerUIManager();

	virtual void Init();
	virtual void Update(double dt);
	void UpdateStats(float dt);
	void UpdateQuestsMenu(float dt);
	virtual void Render();
	virtual void Exit();

private:
	float ButtonTimer;
	float WaitTime;

	Vector3 CenterPosition;
	const static std::string UI_Text[];

	// Unclickable HUD
	UI_System UI_HUD;
	UI_Layer* HUD_Stats;
	UI_Layer* HUD_AddInfo;
	UI_Layer* HUD_Quests;

	void InitHUD();
	void RenderHUD();

	// Clickable Menu
	UI_System UI_Menu;
	
	UI_Layer* Menu_Base;
	std::vector<UI_Element*> Menu_BaseButtons;
	UI_Layer* Menu_Stats;
	UI_Layer* Menu_Inventory;
	UI_Layer* Menu_Quests;
	UI_Element* QuestLeftPointer;
	UI_Element* QuestRightPointer;
	Quest* CurrentQuest;
	QuestStage* CurrentStage;
	std::vector<UI_Element*> Menu_QuestTextElements;
	UI_Layer* Menu_Save;

	int QuestDisplayNumber = 0;
	int CurrentQuestDisplayNumber = -1;

	std::map<int, Quest*> ActiveQuestList;

	void InitMenu();
	void RenderMenu();
	void UpdateStatsHUD(float dt);
};

#endif // _PLAYER_UI_MANAGER_H