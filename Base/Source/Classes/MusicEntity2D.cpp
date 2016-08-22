#include "MusicEntity2D.h"
#include "MyMath.h"

MusicEntity2D::MusicEntity2D()
    : GenericEntity()
{
    SoundEffects = nullptr;
    SoundSource = nullptr;
    volume_ = 1.f;
}

MusicEntity2D::~MusicEntity2D()
{

}

void MusicEntity2D::SetVolume(const float &theNum)
{
    volume_ = theNum;
    volume_ = Math::Clamp(volume_, 0.01f, 1.f);
}

void MusicEntity2D::Init()
{

}

bool MusicEntity2D::onNotify(const std::string &theEvent)
{

    return true;
}