#include "MusicSystem.h"
#include <fstream>
#include "../Classes/MusicEntity3D.h"
#include "Scene_System.h"
#include <sstream>
#include <set>
#include "../Scenes/GraphicsEntity.h"

void MusicSystem::Init()
{
    musicEngine = createIrrKlangDevice();
    theOnlyBackgroundMusic = nullptr;
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
        MusicEntity2D *theMusic = nullptr;
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
                {
                    convertStringToUpperCaps(token);
                    keys.push_back(token);
                }
            }
            else {
                while (getline(iss, token, ','))
                    values.push_back(token);

                std::vector<std::string>::iterator it;
                it = std::find(keys.begin(), keys.end(), "SOUNDTYPE");
                size_t pos = it - keys.begin();
                if (values[pos].find("3D") != std::string::npos)
                    theMusic = new MusicEntity3D();
                else if (values[pos] != "")
                    theMusic = new MusicEntity2D();
                if (theMusic) {
                    it = std::find(keys.begin(), keys.end(), "SOUNDFILENAME");
                    pos = it - keys.begin();
                    theMusic->setName(values[pos]);


                }
            }
            all_the_Music.insert(std::pair<std::string, MusicEntity2D*>(theMusic->getName(), theMusic));
            theMusic = nullptr;
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

void MusicSystem::clearEverything()
{
    for (auto it : all_the_Music)
    {
        delete it.second;
    }
    all_the_Music.clear();
    theOnlyBackgroundMusic = nullptr;
    musicEngine->drop();
}