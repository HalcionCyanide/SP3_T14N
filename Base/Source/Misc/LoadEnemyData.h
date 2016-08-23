#ifndef LOAD_EDATA_H
#define LOAD_EDATA_H

#include <map>
#include "Vector3.h"
#include <string>
#include "../Classes/Enemy.h"

bool LoadEnemyData(const char *file_path, std::map<std::string, Enemy*> &EMap);

#endif