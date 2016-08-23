#ifndef _SCENE_BATTLE_SCREEN_H
#define _SCENE_BATTLE_SCREEN_H

#include "..\\Classes\\SceneEntity.h"
#include "..\\Systems\\Scene_System.h"
/****************************************************************************/
/*!
\file SceneBattleScreen.cpp
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the battle screen scene
*/
/****************************************************************************/

#include "GraphicsEntity.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\CameraBS.h"
#include "..\\Systems\\UI_System.h"
#include "..\\Classes\\BaseObject.h"

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

	double framerates;
	GenericEntity *theInteractiveMap;
};

#endif //_SCENE_BATTLE_SCREEN_H