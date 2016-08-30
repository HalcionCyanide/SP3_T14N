#ifndef _PLAYER_UI_MANAGER_H
#define _PLAYER_UI_MANAGER_H

#include "UI_System.h"

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
	virtual void Render();
	virtual void Exit();

private:
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
	UI_Layer* Menu_Save;

	// UI Elems for menu
	UI_Element* Tab_StatsButton;
	UI_Element* Tab_InventoryButton;
	UI_Element* Tab_QuestsButton;
	UI_Element* Tab_SaveButton;

	void InitMenu();
	void RenderMenu();
	void UpdateStatsHUD(float dt);
};

#endif // _PLAYER_UI_MANAGER_H