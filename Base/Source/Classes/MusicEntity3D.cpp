#include "MusicEntity3D.h"
#include "../Systems/MusicSystem.h"

MusicEntity3D::MusicEntity3D()
{
    name_ = "";
    SoundSource = nullptr;
    volume_ = 1.f;
    maxTimeToPlay = 1;
    unlimitedTimes = false;
    position.set(0, 0, 0);
}

MusicEntity3D::MusicEntity3D(const std::string &theName, const vec3df &thePos, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes)
{
    Init(theName, thePos, theVol, playHowManyTime, unlimitedTimes);
}

MusicEntity3D::~MusicEntity3D()
{

}

void MusicEntity3D::Play()
{
    if (HistoryOfPlayTimes.size() < maxTimeToPlay)
    {

    }
}

vec3df MusicEntity3D::convertFromVectorToSuitableIrrklangVector(const Vector3 &theVec)
{
    return vec3df(theVec.x, theVec.y, theVec.z);
}

void MusicEntity3D::Init(const std::string &theName, const vec3df &thePos, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes)
{

}