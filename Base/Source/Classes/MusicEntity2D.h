#ifndef MUSIC_ENTITY_2D_H
#define MUSIC_ENTITY_2D_H

#include "GenericEntity.h"
#include "irrklang.h"
#include <queue>
using namespace irrklang;

class MusicEntity2D : public GenericEntity
{
public:
    MusicEntity2D();
    virtual ~MusicEntity2D();

    virtual void Init(const std::string &theName, const float &theVol, const int &playHowManyTime = 1, const bool &unlimitedTimes = false);
    virtual void Play();
    virtual void Update(double dt);
    virtual void SetVolume(const float &theNum);
    virtual bool onNotify(const std::string &theEvent);

protected:
    ISoundSource *SoundSource;
    float volume_;
    std::queue<ISound*> HistoryOfPlayTimes;
    unsigned maxTimeToPlay;
    bool unlimitedTimes;
};

#endif