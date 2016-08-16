#pragma once
#include <string>
#include <vector>
#include <iostream>

class Quest
{
public:
	Quest();
	~Quest();

	std::string getName();
	void setName(std::string);

	int getStage();
	void setStage(int);
	void nextStage();

	int getMaxStages();

	std::vector<std::string>& getDesc();

	bool getActive();
	void setActive(bool);

	friend std::ostream& operator<<(std::ostream& os, const Quest& quest);
private:
	int id = 0;
	std::string questName;
	std::vector<std::string> questDesc;
	int currentStage;
	bool active;
};

