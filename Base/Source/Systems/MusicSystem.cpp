#include "MusicSystem.h"

MusicSystem::MusicSystem()
    : musicEngine(nullptr)
{

}

MusicSystem::~MusicSystem()
{
    if (musicEngine)
        musicEngine->drop();
}

void MusicSystem::Init()
{

}

void MusicSystem::Update(double dt)
{

}

void MusicSystem::onNotify(const std::string &theMessage)
{

}