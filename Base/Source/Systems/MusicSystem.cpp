#include "MusicSystem.h"
#include <fstream>
#include "../Classes/MusicEntity3D.h"
#ifdef _DEBUG
    #include <assert.h>
#endif

MusicSystem::MusicSystem()
    : musicEngine(nullptr)
{
    Init();
}

MusicSystem::~MusicSystem()
{
    if (musicEngine)
        musicEngine->drop();
}

void MusicSystem::Init()
{
    musicEngine = createIrrKlangDevice();
#ifdef _DEBUG
    assert(beginLoadingMusic("Image//MusicDriven.csv"));
#else
    beginLoadingMusic("Image//MusicDriven.csv");
#endif
}

void MusicSystem::Update(double dt)
{

}

void MusicSystem::onNotify(const std::string &theMessage)
{

}

bool MusicSystem::beginLoadingMusic(const std::string &fileName)
{

    return false;
}