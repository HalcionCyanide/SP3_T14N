#pragma once

#include "GameObject.h"
#include "Scene_System.h"

class PlayerObject : public GameObject
{
public:
    PlayerObject();
    virtual ~PlayerObject();

protected:
    Vector3 vel_, accel;
};