/******************************************************************************/
/*!
\file           Scene_MainMenu.h
\author Ryan Lim Rui An, Lee Sek Heng
\par email: 150577L@mymail.nyp.edu.sg, 150629Z@mymail.nyp.edu.sg
\brief
The Main Menu Scene for the game
*/
/******************************************************************************/
#ifndef _SCENE_MAINMENU_H
#define _SCENE_MAINMENU_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"
#include "..\\Systems\\InputManager.h"
#include "..\\Classes\\Camera3.h"
#include "..\\Mains\\Application.h"
#include "..\\Misc\\Utility.h"
#include <vector>
#include "..\\Systems\\BillboardManager.h"
#include "..\\Systems\\UI_System.h"

class Scene_MainMenu : public SceneEntity
{
public:
	static std::string id_;
	Scene_MainMenu();
	virtual ~Scene_MainMenu();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Render();
	virtual void Exit();

	Camera3 camera;


private:
	enum STATE_MAIN_MENU
	{
		S_FIRSTLEVEL,
		S_SECONDLEVEL,
        S_SETTING,
        S_UPDATING_KEYS,
        S_LOADING_SAVE,
        S_SETTING_KEYS,
        S_TOTAL,
	};

	STATE_MAIN_MENU CurrentMenuState = S_FIRSTLEVEL;

	const static std::string UI_Text[];

	void InitSceneUIElems();
	void UpdateUILogic(float dt, Scene_MainMenu::STATE_MAIN_MENU);

	// Shadow GPass
	void RenderPassGPass();
	void RenderPassMain();

	//Heightmap stuff
	std::vector<unsigned char> m_heightMap;
	Vector3 TerrainScale;

	// Additional Calls
	void RenderShadowCasters();
	void RenderTerrain();
	void RenderSkybox();

	const float SkyboxSize = 1000;

	BillboardManager BManager;
	UI_System UI_Sys;

	double framerates;

	GenericEntity *theInteractiveMap;
	std::vector<GenericEntity*> testingRenderingStuff;
    std::string whatKeyToChange;
    void writeToGlobalDrivenAndChangeCommand(const unsigned char &command, const std::string &theKey);

    std::string transitingSceneName;
    UI_Layer *NewL, *Setting1, *SettingKeys;
    UI_Element *theLogo;
    std::map<UI_Element*, UI_Element*> all_the_Command_Keys;
    void checkThroughTheCharAndChangingThemToASuitableWord(std::string &theChar);
};

#endif //_SCENE_MAINMENU_H