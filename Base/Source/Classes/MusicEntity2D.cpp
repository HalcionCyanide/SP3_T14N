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
    SetNumTimeToPlay(playHowManyTime);
    SetUnlimitedPlayTimes(unlimitedTimes);
    ConstantLooping(loop);
}

bool MusicEntity2D::OnNotify(const std::string &theEvent)
{
    return true;
}

void MusicEntity2D::Play()
{
    if (HistoryOfPlayTimes.size() < maxTimeToPlay || unlimitedTimes == true)
    {
        ISound *thEffect = MusicSystem::accessing().musicEngine->play2D(SoundSource, loopIt, false, true);
        HistoryOfPlayTimes.push(thEffect);
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

void MusicEntity2D::ConstantLooping(const bool &loop)
{
    loopIt = loop;
}

void MusicEntity2D::SetUnlimitedPlayTimes(const bool &playTimes)
{
    unlimitedTimes = playTimes;
}

void MusicEntity2D::SetNumTimeToPlay(const int &numTime)
{
    if (numTime > 0)
        maxTimeToPlay = numTime;
}

void MusicEntity2D::SetPosition(const Vector3 &pos)
{

}

void MusicEntity2D::Stop()
{
    while (HistoryOfPlayTimes.size() > 0)
    {
        ISound *theEffect = HistoryOfPlayTimes.front();
        theEffect->drop();
        theEffect = 0;
        HistoryOfPlayTimes.pop();
    }
}

float MusicEntity2D::getVolume()
{
    return volume_;
}

unsigned MusicEntity2D::getMaxTimesToPlay()
{
    return maxTimeToPlay;
}

bool MusicEntity2D::getLooping()
{
    return loopIt;
}

bool MusicEntity2D::getUnlimitedTimes()
{
    return unlimitedTimes;
}