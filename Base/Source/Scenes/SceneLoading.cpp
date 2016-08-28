#include "SceneLoading.h"
#include "GraphicsEntity.h"
#include "../Systems/Scene_System.h"

SceneLoading::SceneLoading()
{
    theLoadingUISystem = nullptr;
}

SceneLoading::~SceneLoading()
{

}

void SceneLoading::Init()
{

}

void SceneLoading::Update(double dt)
{

}

void SceneLoading::Render()
{

}

void SceneLoading::Exit()
{
    if (theLoadingUISystem)
    {
        delete theLoadingUISystem;
        theLoadingUISystem = nullptr;
    }
}