#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "GenericSystem.h"
//#include "..\\Classes\\SingletonTemplate.h"
#include "../Classes/MusicEntity.h"

class MusicSystem : public GenericSystem
{
public:
    MusicSystem();
    virtual ~MusicSystem();

    virtual void Init();
    virtual void Update(double dt);
    virtual void onNotify(const std::string &theMessage);

private:
    ISoundEngine *musicEngine;
};

#endif