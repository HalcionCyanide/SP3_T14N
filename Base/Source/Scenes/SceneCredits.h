#ifndef SCENE_CREDITS_H
#define SCENE_CREDITS_H

#include "../Classes/SceneEntity.h"
#include "../Classes/Camera3.h"
#include "../Systems/UI_System.h"

class SceneCredits : public SceneEntity {
public:
    static std::string id_;
    SceneCredits();
    virtual ~SceneCredits();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();

    virtual bool onNotify(const std::string &theEvent);

    Camera3 camera;

private:
    // Shadow GPass
    void RenderPassGPass();
    void RenderPassMain();

    double framerates;

    UI_System *TheLoadScreenStuff;
};

#endif