#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "GenericSystem.h"
#include "../Classes/MusicEntity2D.h"
#include <map>

typedef std::map<std::string, MusicEntity2D*> strMEmap;

class MusicSystem : public GenericSystem, public SingletonTemplate<MusicSystem>
{
public:
    virtual void Init();
    //This is gonna be a pain
    virtual void Update(double dt);
    virtual void onNotify(const std::string &theMessage);
  
    ISoundEngine *musicEngine;
    std::map<std::string, MusicEntity2D*> all_the_Music;

    virtual bool playMusic(const std::string &songName);

    virtual bool playBackgroundMusic(const std::string &songName);

    virtual void clearEverything();

private:
    bool beginLoadingMusic(const std::string &fileName);
    MusicEntity2D *theOnlyBackgroundMusic;
    size_t loopingAndFindKey(std::vector<std::string> &theKeys, const std::string &whatyouwant);
};

#endif