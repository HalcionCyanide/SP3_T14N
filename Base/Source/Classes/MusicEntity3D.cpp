#include "MusicEntity3D.h"

MusicEntity3D::MusicEntity3D()
{
    name_ = "";
    SoundEffects = nullptr;
    SoundSource = nullptr;
    position.set(0, 0, 0);
}

MusicEntity3D::~MusicEntity3D()
{

}

void MusicEntity3D::Init()
{

}