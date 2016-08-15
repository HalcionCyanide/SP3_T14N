#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include "GenericEntity.h"
#include "MatrixStack.h"

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