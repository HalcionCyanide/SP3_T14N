#ifndef _SCENE_BATTLE_SCREEN_H
#define _SCENE_BATTLE_SCREEN_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
#include "GraphicsEntity.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\CameraBS.h"
#include "..\\Systems\\BillboardManager.h"
#include "..\\Systems\\UI_System.h"
#include "..\\Misc\\BaseObject.h"

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

	CameraBS camera;

private:
	// Shadow GPass
	void RenderPassGPass();
	void RenderPassMain();

	// Additional Calls
	void PlayerUpdate(float dt);

	BillboardManager BManager;

	double framerates;
	GenericEntity *theInteractiveMap;

	BaseObject* Player = nullptr;
	std::vector<BaseObject*> ProjectileContainer;

	UI_Element* BaseExterior;
	UI_Element* BaseInterior;

	//Player Stuff
	Vector3 CurrentMousePosition;
	float PlayerScale;
	Vector3 ActingForce;
	bool MouseModeSelected = true;
	bool isInvincible = false;
	float IFrameTimer = 0;
	const float FrictionDecrementMultiplier = 0.8f;

};

#endif //_SCENE_BATTLE_SCREEN_H