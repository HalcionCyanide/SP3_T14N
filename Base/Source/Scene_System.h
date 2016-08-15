#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include "GenericSystem.h"
#include "SingletonTemplate.h"
#include "SceneEntity.h"
#include <map>
#include <stack>
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

private:
	std::stack<SceneEntity* > SceneHistory;

    std::map<std::string, SceneEntity*> theSceneList;
    SceneEntity /**currScene,*/ *graphics_scene;
};

#endif