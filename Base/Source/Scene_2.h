#pragma once

#include "SceneEntity.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"
#include "InputManager.h"
#include "Camera3.h"
#include "Application.h"
#include "Utility.h"
#include <vector>
#include "BillboardManager.h"
#include "GameObject.h"

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
};