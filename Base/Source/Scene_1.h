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

class Scene_1 : public SceneEntity
{
public:
    static std::string id_;
    Scene_1();
    virtual ~Scene_1();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();

	InputManager *SceneInputManager;
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
	void RenderSkyplane();
	void RenderSkybox();

	const float SkyboxSize = 1000;

	BillboardManager BManager;

    double framerates;

    GenericEntity *theInteractiveMap;
};