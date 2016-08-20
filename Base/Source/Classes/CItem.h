#ifndef CITEM_H
#define CITEM_H

#include "GenericEntity.h"

class CItem : public GenericEntity
{
public:
	CItem();
	virtual ~CItem();

	virtual void setDurability(const int &theNum);
	virtual void receiveDamage(const int &theNum);
	virtual void addMoreDurability(const int &theNum);

	virtual void UseIt() = 0;

	int getDurability();

protected:
	int durability_;
};

#endif