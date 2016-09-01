/******************************************************************************/
/*!
\file           SceneCredit.h
\author Lee Sek Heng
\par email: 150577L@mymail.nyp.edu.sg, 150629Z@mymail.nyp.edu.sg
\brief
The Starting Scene of the game which rolls the story and the credits
*/
/******************************************************************************/
#ifndef SCENE_CREDITS_H
#define SCENE_CREDITS_H

#include "../Classes/SceneEntity.h"
#include "..\\Classes\\CameraBS.h"
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

    CameraBS camera;

private:
    // Shadow GPass
    void RenderPassGPass();
    void RenderPassMain();

    double framerates;

    UI_System *TheLoadScreenStuff;
    UI_Layer *SkipCreditStuff, *ComicLayer, *CreditsLayer;
    Vector3 ComicVel, CreditVel;
};

#endif