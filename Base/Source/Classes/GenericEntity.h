#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "..\\Misc\\DetectMemoryLeak.h"
//#include "GenericComponent.h"
#include <string>

class GenericEntity
{
public:
    GenericEntity() { name_ = ""; };
    virtual ~GenericEntity() {};

    virtual void setName(const std::string &zeName) { name_ = zeName; };
    virtual bool onNotify(const std::string &theEvent) { return false; };
    virtual std::string getName() { return name_; };

protected:
    std::string name_;
};

#endif