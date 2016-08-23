#include <iostream>
#include <fstream>
#include <sstream>
#include "LoadEnemyData.h"
#include <vector>
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

bool LoadEnemyData(const char *file_path, std::map<std::string, Enemy*> &EMap)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys;
	std::vector<std::string> CSV_Values;

	while (getline(fileStream, CurrentLineOfText))
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r');
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty())
		{
			// Store the key values if the keys have yet to be stored
			while (getline(iss, CurrentFragmentedSubString, ','))
			{
				convertStringToUpperCaps(CurrentFragmentedSubString);
				CSV_Keys.push_back(CurrentFragmentedSubString);
			}
		}
		else // Commence storing the data within the CSV file
		{
			Enemy* Temp = new Enemy();
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}
			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "ENEMYID");
			size_t pos = it - CSV_Keys.begin();
			Temp->setName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "ENEMYNAME");
			pos = it - CSV_Keys.begin();
			Temp->MeshName = CSV_Values[pos];

			GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
			std::map<std::string, Mesh*>::iterator iter = SceneGraphics->meshList.find(Temp->MeshName);
			if (iter != SceneGraphics->meshList.end())
			{
				Temp->EnemyMesh = iter->second;
			}

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "SPELLPOWER");
			pos = it - CSV_Keys.begin();
			Temp->SpellPower = stoi(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MAXENEMYWAVE");
			pos = it - CSV_Keys.begin();
			Temp->MaxEnemyWave = stoi(CSV_Values[pos]);

			int num = 1;
			while (CSV_Values.size() - 1 > pos)
			{
				std::ostringstream ss;
				EnemyProjectile* TempP = new EnemyProjectile();
				ss << "P" << num << "MESHNAME";
				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
				pos = it - CSV_Keys.begin();
				if (CSV_Values.size() > pos)
				if (CSV_Values[pos] != "")
				{
					GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
					std::map<std::string, Mesh*>::iterator iter = SceneGraphics->meshList.find(CSV_Values[pos]);
					if (iter != SceneGraphics->meshList.end())
					{
						TempP->StoredMesh = iter->second;
					}

					ss.str("");
					ss << "P" << num << "TYPE";
					it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
					pos = it - CSV_Keys.begin();
					if (CSV_Values.size() > pos)
					if (CSV_Values[pos] != "")
					{
						TempP->AttackType = CSV_Values[pos];

						ss.str("");
						ss << "P" << num << "ATTACKSPEED";
						it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
						pos = it - CSV_Keys.begin();
						if (CSV_Values.size() > pos)
						if (CSV_Values[pos] != "")
						{
							TempP->AttackSpeed = stof(CSV_Values[pos]);

							ss.str("");
							ss << "P" << num << "ATTACKSPERWAVE";
							it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
							pos = it - CSV_Keys.begin();
							if (CSV_Values.size() > pos)
							if (CSV_Values[pos] != "")
							{
								TempP->AttacksPerWave = stoi(CSV_Values[pos]);

								ss.str("");
								ss << "P" << num << "DAMAGEPERATTACK";
								it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
								pos = it - CSV_Keys.begin();
								if (CSV_Values.size() > pos)
								if (CSV_Values[pos] != "")
								{
									TempP->DamagePerAttack = stoi(CSV_Values[pos]);

									ss.str("");
									ss << "P" << num << "SCALARACCELERATION";
									it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
									pos = it - CSV_Keys.begin();
									if (CSV_Values.size() > pos)
									if (CSV_Values[pos] != "")
									{
										TempP->ScalarAcceleration = stof(CSV_Values[pos]);
										ss.str("");
										ss << "P" << num << "BATCHCREATECOUNT";
										it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
										pos = it - CSV_Keys.begin();
										if (CSV_Values.size() > pos)
											if (CSV_Values[pos] != "")
											{
												TempP->BatchCreateCount = stoi(CSV_Values[pos]);
												Temp->cE_Projectiles.push_back(TempP);
												EMap.insert(std::pair<std::string, Enemy*>(Temp->getName(), Temp));
											}
									}
								}
							}
						}
					}
				}
				num++;
			}
			CSV_Values.clear();
		}
	}
	fileStream.close();
	return true;
}

bool removingSpecificCharInStr(std::string &theStr, const char &theChar)
{
	for (size_t num = 0, sizeOfStr = theStr.size(); num < sizeOfStr; ++num)
	{
		if (theStr[num] == theChar)
		{
			theStr.erase(num);
			return true;
			break;
		}
	}
	return false;
}