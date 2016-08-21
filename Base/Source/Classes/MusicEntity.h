#ifndef MUSIC_ENTITY_H
#define MUSIC_ENTITY_H

#include "GenericEntity.h"


class MusicEntity : public GenericEntity
{
public:
    MusicEntity();
    virtual ~MusicEntity();

    bool play3D;
};

#endif