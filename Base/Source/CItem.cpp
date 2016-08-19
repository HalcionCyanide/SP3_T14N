#include "CItem.h"

CItem::CItem()
    : GenericEntity()
{
    durability_ = 0;
}

CItem::~CItem()
{

}

void CItem::setDurability(const int &theNum)
{
    durability_ = theNum;
}

int CItem::getDurability()
{
    return durability_;
}

void CItem::receiveDamage(const int &theNum)
{
    durability_ -= theNum;
}

void CItem::addMoreDurability(const int &theNum)
{
    durability_ += theNum;
}