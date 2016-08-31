#ifndef COLLECTION_CONDITION_H_
#define COLLECTION_CONDITION_H_
#include "genericCondition.h"

class collectionCondition : public genericCondition
{
public:
	collectionCondition();
	collectionCondition(std::string);
	virtual ~collectionCondition();

	virtual void setValue(std::string);
	virtual void Update(double dt);
private:
	int counter;
    std::string theRequirement;
};

#endif