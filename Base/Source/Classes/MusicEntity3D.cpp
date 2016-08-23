#include "MusicEntity3D.h"
#include "../Systems/MusicSystem.h"

MusicEntity3D::MusicEntity3D()
{
    name_ = "";
    SoundSource = nullptr;
    volume_ = 1.f;
    maxTimeToPlay = 1;
    unlimitedTimes = loopIt = false;
    position.set(0, 0, 0);
}

MusicEntity3D::MusicEntity3D(const std::string &theName, const std::string &fileName, const Vector3 &thePos, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes, const bool &loop)
{
    Init(theName, fileName, thePos, theVol, playHowManyTime, unlimitedTimes, loop);
}

MusicEntity3D::~MusicEntity3D()
{

}

void MusicEntity3D::Play()
{
    if (HistoryOfPlayTimes.size() < maxTimeToPlay)
    {
        ISound *theEffect = MusicSystem::accessing().musicEngine->play3D(SoundSource, position, loopIt, false, true);
        HistoryOfPlayTimes.push(theEffect);
    }
}

vec3df MusicEntity3D::convertFromVectorToSuitableIrrklangVector(const Vector3 &theVec)
{
    return vec3df(theVec.x, theVec.y, theVec.z);
}

void MusicEntity3D::Init(const std::string &theName, const std::string &fileName, const Vector3 &thePos, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes, const bool &loop)
{
    MusicEntity2D::Init(theName, fileName, theVol, playHowManyTime, unlimitedTimes, loop);
    SetPosition(thePos);
}

void MusicEntity3D::SetPosition(const Vector3 &pos)
{
    position = convertFromVectorToSuitableIrrklangVector(pos);
}