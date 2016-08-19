#ifndef _SCENE_BATTLE_SCREEN_H
#define _SCENE_BATTLE_SCREEN_H

#include "SceneEntity.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"
#include "BattleScreenObject.h"
#include "Camera3.h"
#include "BillboardManager.h"

class SceneBattleScreen : public SceneEntity
{
public:
    static std::string id_;
    SceneBattleScreen();
    virtual ~SceneBattleScreen();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();

    Camera camera;

private:
    // Shadow GPass
    void RenderPassGPass();
    void RenderPassMain();

    // Additional Calls
	void PlayerUpdate(float dt);

    BillboardManager BManager;

    double framerates;
    GenericEntity *theInteractiveMap;

	BattleScreenObject* Player = nullptr;
	std::vector<BattleScreenObject*> ProjectileContainer;

	//Player Stuff
	Vector3 ActingForce;
	bool isInvincible = false;
	float IFrameTimer = 0;
	const float FrictionDecrementMultiplier = 0.9f;

};

#endif //_SCENE_BATTLE_SCREEN_H