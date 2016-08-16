#pragma once
#include <string>
#include <vector>
#include <iostream>

class Quest
{
public:
	Quest();
	~Quest();

	enum questType
	{
		quest_fetch,
		quest_capture,
	};

	questType getType();
	void setType(questType);

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
	questType type;
	int currentStage;
	bool active;
};

