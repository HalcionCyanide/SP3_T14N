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
	virtual ~Quest();

	void setID(int);
	int getID();

	void setDesc(std::string);
	std::string getDesc();

	bool getActive();
	void setActive(bool);

	std::string getGiver();
	void setGiver(std::string);

	std::vector<genericCondition*> getConds();
	void setCondition(std::string, std::string);

	friend std::ostream& operator<<(std::ostream&, const Quest&);

	virtual void Update(double dt);
	bool satisfied;
private:
	int id;
	std::vector<genericCondition*> conditions;
	std::string questDesc;
	std::string giver;
	bool active;
};

