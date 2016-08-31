#ifndef RATING_CONDITION_H_
#define RATING_CONDITION_H_
#include "genericCondition.h"

class ratingCondition : public genericCondition
{
public:
	ratingCondition();
	ratingCondition(std::string);
	virtual ~ratingCondition();

	virtual void Update(double dt);
	virtual void setValue(std::string);
private:
	int counter;
	std::string theRequirement;
};

#endif //RATING_CONDITION_H_