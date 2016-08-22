#include "MusicEntity2D.h"
#include "MyMath.h"
#include "../Systems/MusicSystem.h"

MusicEntity2D::MusicEntity2D()
    : GenericEntity()
{
    SoundSource = nullptr;
    volume_ = 1.f;
    maxTimeToPlay = 1;
    unlimitedTimes = false;
}

MusicEntity2D::~MusicEntity2D()
{

}

void MusicEntity2D::SetVolume(const float &theNum)
{
    volume_ = theNum;
    volume_ = Math::Clamp(volume_, 0.01f, 1.f);
}

void MusicEntity2D::Init(const std::string &theName, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes)
{
    setName(theName);
    SoundSource = 
        MusicSystem::accessing().musicEngine->addSoundSourceFromFile(theName.c_str());
    volume_ = theVol;
    if (playHowManyTime > 0)
        maxTimeToPlay = (unsigned)playHowManyTime;
    this->unlimitedTimes = unlimitedTimes;
}

bool MusicEntity2D::onNotify(const std::string &theEvent)
{

    return true;
}

void MusicEntity2D::Play()
{
    if (HistoryOfPlayTimes.size() < maxTimeToPlay)
    {

    }
}

void MusicEntity2D::Update(double dt)
{
    if (HistoryOfPlayTimes.size() > 0 && HistoryOfPlayTimes.front()->isFinished())
    {
        HistoryOfPlayTimes.pop();
    }
}