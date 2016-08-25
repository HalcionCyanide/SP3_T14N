/******************************************************************************/
/*!
\file        Scene_System.h
\author      Lee Sek Heng
\par         email: 150629Z@mymail.nyp.edu.sg
\brief
A System that handles all the scene as well as most of the global variables.
GraphicsEntity resides here.
*/
/******************************************************************************/
#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include "GenericSystem.h"
#include "..\\Classes\\SingletonTemplate.h"
#include "..\\Classes\\SceneEntity.h"
#include <map>
#include <stack>
#include "InputManager.h"
#include "..\\Classes\\Boundary.h"
#include "..\\Systems\\BattleSystem.h"
#include "..\\Classes\\GlobalPlayer.h"
#include "questManager.h"
#include "NPC_Manager.h"
#include "UI_System.h"

class GlobalPlayer;
class InputManager;
/******************************************************************************/
/*!
Class Scene_System:
\brief  Defines a scene system, it's methods for transitioning between scenes,
            a container of those scenes and the Graphics Scene. Inheirits from GenericSystem and SingletonTemplate
*/
/******************************************************************************/
class Scene_System : public GenericSystem, public SingletonTemplate<Scene_System>
{
public:
    virtual void Init();    
    virtual void Update(double dt);

    virtual void AddScene(SceneEntity&);
    virtual void RemoveScene(SceneEntity&);

	virtual void SwitchScene(const std::string&);
	virtual bool SwitchToPreviousScene();

    SceneEntity &getCurrScene();
    SceneEntity &getGraphicsScene();

	virtual void setGraphics_Scene(SceneEntity&);
    virtual void clearEverything();

	InputManager *cSS_InputManager;

	BattleSystem *BSys;
	std::map<std::string, Enemy*> EnemyData;

    GlobalPlayer* gPlayer;

	questManager QM;
	NPCManager NM;
    UI_System *theLoadingEffect;

private:
    void doingLoadingEffect();
	std::stack<SceneEntity* > SceneHistory;
    std::map<std::string, SceneEntity*> theSceneList;
    SceneEntity *graphics_scene;
};

#endif