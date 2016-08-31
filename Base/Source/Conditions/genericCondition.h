#ifndef GENERICCONDITIONS_H_
#define GENERICCONDITIONS_H_
#include <string>

struct genericCondition
{
	genericCondition(){ complete = false; };
	virtual ~genericCondition(){};

	virtual void setValue(std::string) = 0;
	virtual void Update(double dt) = 0;

	bool complete;
};

#endif