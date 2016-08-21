#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "GenericSystem.h"
#include "..\\Classes\\SingletonTemplate.h"

class MusicSystem : public GenericSystem, public SingletonTemplate<MusicSystem>
{
public:
    virtual void Init();
    virtual void Update(double dt);
};

#endif