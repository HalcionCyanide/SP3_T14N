#ifndef MUSIC_ENTITY_2D_H
#define MUSIC_ENTITY_2D_H

#include "GenericEntity.h"
#include "irrklang.h"
using namespace irrklang;

class MusicEntity2D : public GenericEntity
{
public:
    MusicEntity2D();
    virtual ~MusicEntity2D();

protected:
    ISound *SoundEffects;
    ISoundSource *SoundSource;
};

#endif