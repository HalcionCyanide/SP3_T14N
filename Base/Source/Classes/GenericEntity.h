/******************************************************************************/
/*!
\file           GenericEntity.h
\author      Lee Sek Heng
\par            email: 150629Z@mymail.nyp.edu.sg
\brief
A Generic Entity so that polymorphism will make life easier when all objects inheirits from it.
*/
/******************************************************************************/
#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "..\\Misc\\DetectMemoryLeak.h"
//#include "GenericComponent.h"
#include <string>

/******************************************************************************/
/*!
Class GenericEntity:
\brief  Defines a GenericEntity and it's methods.
*/
/******************************************************************************/
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