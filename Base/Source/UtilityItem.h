#ifndef UTILITY_ITEM_H
#define UTILITY_ITEM_H

#include "CItem.h"

class UtilityItem : public CItem
{
public:
    UtilityItem();
    virtual ~UtilityItem();

    virtual void UseIt();
};

#endif