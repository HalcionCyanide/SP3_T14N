#ifndef GENERIC_SYSTEM_H
#define GENERIC_SYSTEM_H

#include "SingletonTemplate.h"
#include "GenericEntity.h"
#include <string>

class GenericSystem/* : public SingletonTemplate<GenericSystem>*/
{
public:
    virtual void Init() = 0;
    virtual void Update(double dt) = 0;
    
private:
    
};

#endif