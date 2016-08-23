#ifndef LOAD_EDATA_H
#define LOAD_EDATA_H

#include <map>
#include "Vector3.h"
#include <string>
#include "../Classes/Enemy.h"

bool LoadEnemyData(const char *file_path, std::map<std::string, Enemy*> &EMap);
bool removingSpecificCharInStr(std::string &theStr, const char &theChar);
Mesh *getMeshPtr(const std::string &meshName);
Mesh &getMeshRef(const std::string &meshName);

#endif