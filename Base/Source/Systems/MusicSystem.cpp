#include "MusicSystem.h"
#include <fstream>
#include "../Classes/MusicEntity3D.h"
#ifdef _DEBUG
    #include <assert.h>
#endif
#include "Scene_System.h"

void MusicSystem::Init()
{
    musicEngine = createIrrKlangDevice();
#ifdef _DEBUG
    assert(beginLoadingMusic("DrivenFiles//MusicDriven.csv"));
#else
    beginLoadingMusic("Image//MusicDriven.csv");
#endif
}

//This is gonna be a pain
void MusicSystem::Update(double dt)
{
    //How to update listener position
    musicEngine->update();
}

void MusicSystem::onNotify(const std::string &theMessage)
{

}

bool MusicSystem::beginLoadingMusic(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    return false;
}