#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "GenericSystem.h"
#include "../Classes/MusicEntity2D.h"

class MusicSystem : public GenericSystem, SingletonTemplate<MusicSystem>
{
public:
    virtual void Init();
    //This is gonna be a pain
    virtual void Update(double dt);
    virtual void onNotify(const std::string &theMessage);

    

private:
    ISoundEngine *musicEngine;
    bool beginLoadingMusic(const std::string &fileName);
};

#endif