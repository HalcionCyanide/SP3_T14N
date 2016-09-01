/****************************************************************************/
/*!
\file genericCondition.h
\author Zheng Qingping
\par email: 152063Q@mymail.nyp.edu.sg
\brief
Defines the basic requirements for a QUEST CONDITON
*/
/****************************************************************************/
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