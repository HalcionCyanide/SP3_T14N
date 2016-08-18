#ifndef _SCENE_FREE_FIELD_H
#define _SCENE_FREE_FIELD_H


#include "SceneEntity.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"
#include "GameObject.h"
#include "Camera3.h"
#include "BillboardManager.h"

class SceneFreeField : public SceneEntity
{
public:
    static std::string id_;
    SceneFreeField();
    virtual ~SceneFreeField();

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
    GameObject* Player;

    double framerates;
    GenericEntity *theInteractiveMap;
    std::vector<GameObject*> objVec;
};

#endif // _SCENE_FREE_FIELD_H