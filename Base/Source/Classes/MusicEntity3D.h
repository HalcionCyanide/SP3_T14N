#ifndef MUSIC_ENTITY_3D_H
#define MUSIC_ENTITY_3D_H

#include "MusicEntity2D.h"
#include "Vector3.h"

class MusicEntity3D : public MusicEntity2D
{
public:
    MusicEntity3D();
    MusicEntity3D(const std::string &theName, const Vector3 &thePos, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes, const bool &loop);
    virtual ~MusicEntity3D();

    virtual void Init(const std::string &theName, const Vector3 &thePos, const float &theVol, const int &playHowManyTime = 1, const bool &unlimitedTimes = false, const bool &loop = false);
    virtual void Play();

    virtual void SetPosition(const Vector3 &pos);

protected:
    vec3df convertFromVectorToSuitableIrrklangVector(const Vector3 &theVec);
    vec3df position;
};

#endif