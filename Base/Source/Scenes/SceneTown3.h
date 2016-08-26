#ifndef _SCENE_TOWN_3_H
#define _SCENE_TOWN_3_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "GraphicsEntity.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\Camera3.h"
#include "..\\Systems\\BillboardManager.h"

class SceneTown3 : public SceneEntity
{
public:
    static std::string id_;
    SceneTown3();
    virtual ~SceneTown3();

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

    const float SkyboxSize = 1000;

    BillboardManager BManager;
    GameObject* Player;

    double framerates;
    GenericEntity *theInteractiveMap;
    std::vector<GameObject*> objVec;
};

#endif // _SCENE_TOWN_3_H