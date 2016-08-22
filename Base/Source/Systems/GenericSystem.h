#ifndef GENERIC_SYSTEM_H
#define GENERIC_SYSTEM_H

#include "..\\Classes\\SingletonTemplate.h"
#include "..\\Classes\\GenericEntity.h"
#include <string>

class GenericSystem
{
public:
    virtual void Init() = 0;
    virtual void Update(double dt) = 0;
    
private:
    
};

#endif