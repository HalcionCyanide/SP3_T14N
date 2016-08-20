#ifndef GENERICCONDITIONS_H_
#define GENERICCONDITIONS_H_

template<class T> 
struct genericCondition
{
	genericCondition(){};
	genericCondition(const T &data) { value = data; }
	virtual ~genericCondition(){};

	virtual void Update(double dt) = 0;

	T value;
};

#endif