#ifndef _SCENE_TOWN_1_H
#define _SCENE_TOWN_1_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "GraphicsEntity.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\Camera3.h"
#include "..\\Systems\\BillboardManager.h"

class SceneTown1 : public SceneEntity
{
public:
    static std::string id_;
    SceneTown1();
    virtual ~SceneTown1();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();

    virtual bool onNotify(const std::string &theEvent);

    Camera3 camera;
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

	// CHAT UI STUFFS <!>
	void InitChatUI();
	int HandleChatUIInput(float dt);
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
};

#endif // _SCENE_TOWN_1_H