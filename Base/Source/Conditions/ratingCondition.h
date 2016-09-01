/****************************************************************************/
/*!
\file NPC.h
\author Zheng Qingping
\par email: 152063Q@mymail.nyp.edu.sg
\brief
Inherits from the generic condition, completes upon player SP reaching a value
*/
/****************************************************************************/
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