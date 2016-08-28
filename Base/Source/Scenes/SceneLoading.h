#pragma once

#include "../Classes/SceneEntity.h"

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
};