#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "genericCondition.h"
class Quest
{
public:
	Quest();
	~Quest();

	int getID();

	void setName(std::string);
	std::string getName();

	void setDesc(std::string);
	std::string getDesc();

	bool getActive();
	void setActive(bool);

	bool getComplete();
	void setComplete(bool);

	friend std::ostream& operator<<(std::ostream&, const Quest&);
private:
	int id;
	std::string questName;
	std::string questDesc;
	bool active;
	bool completed;
    //template<class T>
    //std::vector<genericCondition<T>*> bunch;
};

