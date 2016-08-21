#ifndef COLLECTION_CONDITION_H_
#define COLLECTION_CONDITION_H_
#include "genericCondition.h"
#include "..\\Classes\\GameObject.h"

class collectionCondition : public genericCondition
{
public:
	collectionCondition();
	collectionCondition(int);
	virtual ~collectionCondition();

	virtual void setValue(std::string);
	virtual void Update(double dt);
private:
	int counter;
	//GameObject* target;
    Data<int> theRequirement;
};

#endif