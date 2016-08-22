#include "MusicSystem.h"
#include <fstream>
#include "../Classes/MusicEntity3D.h"
#include "Scene_System.h"

void MusicSystem::Init()
{
    musicEngine = createIrrKlangDevice();
    beginLoadingMusic("DrivenFiles//MusicDriven.csv");
}

//This is gonna be a pain
void MusicSystem::Update(double dt)
{
    //How to update listener position
    musicEngine->update();
    for (std::map<std::string, MusicEntity2D*>::iterator it = all_the_Music.begin(), end = all_the_Music.end(); it != end; ++it)
    {
        it->second->Update(dt);
    }
}

void MusicSystem::onNotify(const std::string &theMessage)
{
    for (std::map<std::string, MusicEntity2D*>::iterator it = all_the_Music.begin(), end = all_the_Music.end(); it != end; ++it)
    {
        it->second->onNotify(theMessage);
    }
}

bool MusicSystem::beginLoadingMusic(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.is_open())
    {

        file.close();
        return true;
    }
    return false;
}