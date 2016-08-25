#ifndef RATING_CONDITION_H_
#define RATING_CONDITION_H_
#include "genericCondition.h"

class ratingCondition : public genericCondition
{
public:
	ratingCondition();
	ratingCondition(int);
	virtual ~ratingCondition();

	virtual void Update(double dt);
	virtual void setValue(std::string);
private:
	int counter;
	Data<int> theRequirement;
};

#endif //RATING_CONDITION_H_