/******************************************************************************/
/*!
\file   SceneTown2.h
\author Ryan Lim Rui An, Lee Sek Heng, Tan Teck Ling, Zheng Qing Ping
\par email: 150577L@mymail.nyp.edu.sg, 150629Z@mymail.nyp.edu.sg, 152063Q@mymail.nyp.edu.sg
\brief
Town 2 Scene which displays the overview of town 2
*/
/******************************************************************************/
#ifndef _SCENE_TOWN_2_H
#define _SCENE_TOWN_2_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "GraphicsEntity.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\Camera3.h"
#include "..\\Systems\\BillboardManager.h"

class SceneTown2 : public SceneEntity
{
public:
    static std::string id_;
    SceneTown2();
    virtual ~SceneTown2();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();
    virtual bool onNotify(const std::string &theEvent);

    Camera3 *camera;

private:
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

	// CHAT UI STUFFS
	void InitChatUI();
	void NPC_chat(float);
	std::string HandleChatUIInput(float dt);
	UI_Layer* ChatLayer;
	UI_Element* NPC_Name;
	UI_Element* NPC_TextBox;
	std::vector<UI_Element*> NPC_QuestButtons;
	// CHAT UI END

	const float SkyboxSize = 1000;

	BillboardManager BManager;
	GameObject* Player;

	double framerates;
	GenericEntity *theInteractiveMap;
	std::vector<GameObject*> objVec;
	Vector3 CenterPosition;
	UI_System* UI_Sys;
	std::string transitingSceneName;
	std::vector<NPC*> npcInThisScene;
};

#endif // _SCENE_TOWN_2_H