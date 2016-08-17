#pragma once
#include <string>
#include <vector>
#include <iostream>

class Quest
{
public:
	Quest();
	~Quest();

	int getID();

	std::string getName();
	void setName(std::string);

	int getStage();
	void nextStage();

	int getMaxStages();

	std::vector<std::string>& getDesc();

	bool getActive();
	void setActive(bool);

	bool getComplete();
	void setComplete(bool);

	friend std::ostream& operator<<(std::ostream&, const Quest&);
private:
	int id = 0;
	std::string questName;
	std::vector<std::string> questDesc;
	int currentStage;
	bool active;
	bool completed;
};

