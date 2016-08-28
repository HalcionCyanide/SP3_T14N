#pragma once

#include "../Classes/SceneEntity.h"
#include "../Systems/UI_System.h"

class SceneLoading : public SceneEntity
{
public:
    SceneLoading();
    virtual ~SceneLoading();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

private:
    UI_System *theLoadingUISystem;
};