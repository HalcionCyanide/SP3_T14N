#include "MusicEntity2D.h"
#include "MyMath.h"
#include "../Systems/MusicSystem.h"

MusicEntity2D::MusicEntity2D()
    : GenericEntity()
{
    SoundSource = nullptr;
    volume_ = 1.f;
    maxTimeToPlay = 1;
    unlimitedTimes = loopIt = false;
}

MusicEntity2D::~MusicEntity2D()
{
    while (HistoryOfPlayTimes.size() > 0)
    {
        ISound *theFront = HistoryOfPlayTimes.front();
        if (theFront)
        {
            theFront->drop();
            theFront = 0;
        }
    }
}

void MusicEntity2D::SetVolume(const float &theNum)
{
    volume_ = theNum;
    volume_ = Math::Clamp(volume_, 0.01f, 1.f);
}

void MusicEntity2D::Init(const std::string &theName, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes, const bool &loop)
{
    setName(theName);
    SoundSource = 
        MusicSystem::accessing().musicEngine->addSoundSourceFromFile(theName.c_str());
    volume_ = theVol;
    setNumTimeToPlay(playHowManyTime);
    setUnlimitedPlayTimes(unlimitedTimes);
    constantLooping(loop);
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
        ISound *theFront = HistoryOfPlayTimes.front();
        HistoryOfPlayTimes.pop();
        if (theFront)
        {
            theFront->drop();
            theFront = 0;
        }
    }
}

void MusicEntity2D::constantLooping(const bool &loop)
{
    loopIt = loop;
}

void MusicEntity2D::setUnlimitedPlayTimes(const bool &playTimes)
{
    unlimitedTimes = playTimes;
}

void MusicEntity2D::setNumTimeToPlay(const int &numTime)
{
    if (numTime > 0)
        maxTimeToPlay = numTime;
}