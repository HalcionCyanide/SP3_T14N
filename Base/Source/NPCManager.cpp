#include "NPCManager.h"

NPCManager::NPCManager()
{
}

NPCManager::~NPCManager()
{

}

void NPCManager::readFile(const std::string fileName)
{
	std::string temp;

	std::ifstream file(fileName.c_str());

	if (!file.is_open())
	{
		std::cout << "Impossible to open " << fileName << ". Are you in the right directory ?\n";
	}
	while (file.good())
	{
		std::string temp;
		//<!>change stuff, plan the file format first
		if (std::getline(file, temp))
		{
			if (temp[0] == '[')
			{
			}
			else if (temp[0] == ']')
			{
			}
			else
			{
			}
			temp.clear();
		}
	}
}