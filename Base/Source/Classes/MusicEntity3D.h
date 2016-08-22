#ifndef MUSIC_ENTITY_3D_H
#define MUSIC_ENTITY_3D_H

#include "MusicEntity2D.h"

class MusicEntity3D : public MusicEntity2D
{
public:
    MusicEntity3D();

    virtual ~MusicEntity3D();

    virtual void Init();

protected:
    vec3df position;
};

#endif