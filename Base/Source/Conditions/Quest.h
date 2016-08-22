#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "..\\Classes\\genericEntity.h"
#include "genericCondition.h"

class Quest : public GenericEntity
{
public:
	Quest();
	~Quest();

	void setID(int);
	int getID();

	void setDesc(std::string);
	std::string getDesc();

	bool getActive();
	void setActive(bool);

	void setCondition(std::string, std::string);

	friend std::ostream& operator<<(std::ostream&, const Quest&);
private:
	int id;
	std::vector<genericCondition*> conditions;
	std::string questDesc;
	bool active;
};

