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
        HistoryOfPlayTimes.pop();
    }
}

void MusicEntity2D::SetVolume(const float &theNum)
{
    volume_ = theNum;
    volume_ = Math::Clamp(volume_, 0.01f, 1.f);
    SoundSource->setDefaultVolume(volume_);
}

void MusicEntity2D::Init(const std::string &theName, const std::string &fileName, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes, const bool &loop)
{
    setName(theName);
    setISoundSouce(fileName);
    SetVolume(theVol);
    SetNumTimeToPlay(playHowManyTime);
    SetUnlimitedPlayTimes(unlimitedTimes);
    SetConstantLooping(loop);
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

void MusicEntity2D::SetConstantLooping(const bool &loop)
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
        if (theEffect)
        {
            theEffect->stop();
            theEffect->drop();
            theEffect = 0;
        }
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

void MusicEntity2D::setISoundSouce(const std::string &theFile)
{
    fileName = theFile;
    SoundSource =
        MusicSystem::accessing().musicEngine->addSoundSourceFromFile(fileName.c_str());
}