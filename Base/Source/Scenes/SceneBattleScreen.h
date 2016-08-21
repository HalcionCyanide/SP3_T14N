#ifndef _SCENE_BATTLE_SCREEN_H
#define _SCENE_BATTLE_SCREEN_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "GraphicsEntity.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\Camera3.h"
#include "..\\Systems\\BillboardManager.h"
#include "..\\Systems\\UI_System.h"
#include "..\\Misc\\BattleScreenObject.h"

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

	UI_Element* BaseExterior;
	UI_Element* BaseInterior;

	//Player Stuff
	Vector3 CurrentMousePosition;
	float PlayerScale;
	Vector3 ActingForce;
	bool isInvincible = false;
	float IFrameTimer = 0;
	const float FrictionDecrementMultiplier = 0.8f;

};

#endif //_SCENE_BATTLE_SCREEN_H