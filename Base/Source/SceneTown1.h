#pragma once

#include "SceneEntity.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"
#include "GameObject.h"
#include "Camera3.h"
#include "BillboardManager.h"

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

    const float SkyboxSize = 1000;

    BillboardManager BManager;

    double framerates;
    GenericEntity *theInteractiveMap;
    std::vector<GameObject*> objVec;
};