#include "MusicSystem.h"
#include <fstream>
#include "../Classes/MusicEntity3D.h"
#include "Scene_System.h"
#include <sstream>
#include <set>

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
        std::string data = "";
        std::vector<std::string> keys;
        std::vector<std::string> values;
        std::set<std::string> keepingTrackOfFileNames;
        while (getline(file, data))
        {
            if (data == "")
                continue;
            std::string token = "";
            std::istringstream iss(data);
            if (keys.empty())
            {
                while (getline(iss, token, ','))
                    keys.push_back(token);
            }
            else {
                while (getline(iss, token, ','))
                    values.push_back(token);

            }
        }

        file.close();
        return true;
    }
    return false;
}

bool MusicSystem::playBackgroundMusic(const std::string &songName)
{
    strMEmap::iterator it = all_the_Music.find(songName);
    if (it != all_the_Music.end())
    {
        it->second->Play();
        return true;
    }
    return false;
}

bool MusicSystem::playMusic(const std::string &songName)
{
    strMEmap::iterator it = all_the_Music.find(songName);
    if (it != all_the_Music.end())
    {
        it->second->Play();
        return true;
    }
    return false;
}