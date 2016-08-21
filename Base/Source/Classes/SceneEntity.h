/******************************************************************************/
/*!
\file           SceneEntity.h
\author      Lee Sek Heng
\par            email: 150629Z@mymail.nyp.edu.sg
\brief
A basic Scene Entity inspired from Scene
However, this time the Mtx44 are pointer so that GraphicsEntity and SceneSystem will have an easier time handling it.
*/
/******************************************************************************/
#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include "GenericEntity.h"
#include "MatrixStack.h"

/******************************************************************************/
/*!
Class SceneEntity:
\brief  Defines a scene and it's method.
*/
/******************************************************************************/
class SceneEntity : public GenericEntity
{
public:
    SceneEntity() : GenericEntity() {
        modelStack = new MS();
        viewStack = new MS();
        projectionStack = new MS();
    };
    virtual ~SceneEntity() {
        if (modelStack)
            delete modelStack;
        if (viewStack)
            delete viewStack;
        if (projectionStack)
            delete projectionStack;
    };

    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Exit() = 0;

    MS *modelStack;
    MS *viewStack;
    MS *projectionStack;

private:
};

#endif