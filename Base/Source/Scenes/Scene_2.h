#pragma once

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "GraphicsEntity.h"
#include "..\\Systems\\InputManager.h"
#include "..\\Classes\\Camera3.h"
#include "..\\Mains\\Application.h"
#include "..\\Misc\\Utility.h"
#include <vector>
#include "..\\Systems\\BillboardManager.h"
#include "..\\Systems\\UI_System.h"
#include "..\\Classes\\GameObject.h"

class Scene_2 : public SceneEntity
{
public:
    static std::string id_;
    Scene_2();
    virtual ~Scene_2();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();

	Camera3 camera;
private:
	// Shadow GPass
	void RenderPassGPass();
	void RenderPassMain();

	//Heightmap stuff
	std::vector<unsigned char> m_heightMap;
	float TerrainYScale = 100.f;
	float TerrainXScale = 700.f;

	// Additional Calls
	void RenderShadowCasters();
	void RenderTerrain();
	void RenderSkybox();

	const float SkyboxSize = 1000;

	std::vector<Billboard> StaticBillBoardVec;
	std::vector<Vector3> TreePosVec;
	BillboardManager BManager;

	std::vector<GameObject*> ObjectVec;
	GameObject* Player;
    double framerates;

	UI_System UI_Sys;
};