#ifndef GENERICCONDITIONS_H_
#define GENERICCONDITIONS_H_
#include <string>

template<class Type>
struct Data
{
    Data(const Type& data) { value_ = data; }
    ~Data() {}
    Type value_;

	bool operator>=(const Type& rhs)
	{
		if (value_ >= rhs)
			return true;
		else
			return false;
	}
};

struct genericCondition
{
	genericCondition(){ satisfied = false; };
	virtual ~genericCondition(){};

	virtual void setValue(std::string) = 0;
	virtual void Update(double dt) = 0;

	bool satisfied;
};

#endif