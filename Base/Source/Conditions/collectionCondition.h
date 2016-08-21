#ifndef COLLECTION_CONDITION_H_
#define COLLECTION_CONDITION_H_
#include "genericCondition.h"
#include "..\\Classes\\GameObject.h"

class collectionCondition : public genericCondition
{
public:
	collectionCondition();
	virtual ~collectionCondition();

	virtual void Update(double dt);
private:
	GameObject* target;
    Data<int> theRequirement;
};

#endif