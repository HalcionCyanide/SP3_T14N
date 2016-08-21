#ifndef GENERICCONDITIONS_H_
#define GENERICCONDITIONS_H_

template<class Type>
struct Data
{
    Data(const Type& data) { value_ = data; }
    ~Data() {}
    Type value_;
};

struct genericCondition
{
	genericCondition(){};
	//genericCondition(const T &data) { value = data; }
	virtual ~genericCondition(){};

	virtual void Update(double dt) = 0;
};

#endif