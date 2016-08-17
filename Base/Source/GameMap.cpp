#include "GameMap.h"
#include <fstream>
#include <sstream>
#include "GraphicsEntity.h"
#include <assert.h>
#include "GameObject.h"
#include "Scene_System.h"
#include <algorithm>
#include "LoadHmap.h"

std::map<unsigned char, GenericEntity*> GameMap::bunchOfLegends;

GameMap::GameMap(void)
    : theNumOfTiles_MapHeight(0)
    , theNumOfTiles_MapWidth(0)
    , theTileSize(0)
{
    theScreenMap.clear();
    tileSizeXYZ.SetZero();

    if (bunchOfLegends.empty())
    {
        loadThoseLegends("Image//MapLegend.csv");
    }
}

GameMap::~GameMap(void)
{
    theScreenMap.clear();
    if (bunchOfLegends.empty() == false)
    {
        for (auto it : bunchOfLegends)
            delete it.second;
        bunchOfLegends.clear();
    }
}

bool GameMap::LoadMap(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GenericEntity*> &theRenderingStuff)
{
    if (LoadFile(mapName, theHeightMap, terrainSize, theRenderingStuff) == true)
    {
        printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
        return true;
    }

    return false;
}

bool GameMap::LoadFile(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GenericEntity*> &theRenderingStuff)
{
    //<?>
    int theLineCounter = 0;
    GraphicsEntity *theGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

    std::ifstream file(mapName.c_str());
    assert(file.is_open());
    int i = 0, k = 0;
    while (file.good())
    {
        std::string aLineOfText = "";
        getline(file, aLineOfText);

        if (aLineOfText == "")
            continue;

        std::string token;
        std::istringstream iss(aLineOfText);
        theScreenMap.insert(std::pair< unsigned, std::vector<unsigned char> >(theLineCounter, std::vector<unsigned char>()));
        std::map<unsigned, std::vector<unsigned char>>::iterator it = theScreenMap.find((unsigned)theLineCounter);
        unsigned num_Column = 0;
        while (getline(iss, token, ','))
        {
            std::map<unsigned char, GenericEntity*>::iterator itLegend = bunchOfLegends.find(token[0]);
            if (itLegend != bunchOfLegends.end())
            {
                GameObject *the3Dobject = dynamic_cast<GameObject*>(itLegend->second);
                GameObject *aCopyOfIt = new GameObject(*the3Dobject);
                aCopyOfIt->SetPos(Vector3((float)num_Column, 0, (float)theLineCounter));
                if (token.size() > 1)
                {
                    std::string strRotation = token.substr(2);
                    aCopyOfIt->SetRotation(stof(strRotation), Vector3(0, 1, 0));
                }
                theRenderingStuff.push_back(aCopyOfIt);
                it->second.push_back(itLegend->first);
            }
            else 
              it->second.push_back(' ');
            ++num_Column;
        }
        theLineCounter++;
    }
    theNumOfTiles_MapWidth = theScreenMap.begin()->second.size();
    theNumOfTiles_MapHeight = theLineCounter;

    tileSizeXYZ.x = terrainSize.x / theNumOfTiles_MapWidth;
    tileSizeXYZ.z = terrainSize.z / theNumOfTiles_MapHeight;
    //<?>

    for (std::vector<GenericEntity*>::iterator it = theRenderingStuff.begin(), end = theRenderingStuff.end(); it != end; ++it)
    {
        GameObject *the3DObj = dynamic_cast<GameObject*>(*it);
        the3DObj->SetPos(Vector3(
            ((the3DObj->GetPos().x - ((float)theNumOfTiles_MapWidth / 2.f)) * tileSizeXYZ.x) + (tileSizeXYZ.x * 0.5f),
            0,
            (-(the3DObj->GetPos().z - ((float)theNumOfTiles_MapHeight / 2.f)) * tileSizeXYZ.z) - (tileSizeXYZ.z * 0.5f)
            ));
        the3DObj->SetPos(Vector3(the3DObj->GetPos().x,
            (the3DObj->GetScale().y*0.5f) + terrainSize.y * ReadHeightMap(theHeightMap, the3DObj->GetPos().x / terrainSize.x, the3DObj->GetPos().z / terrainSize.z),
            the3DObj->GetPos().z
            ));
    }

    return true;
}

int GameMap::GetTileSize(void)
{
    return theTileSize;
}

// Get the number of tiles for height of the map
int GameMap::getNumOfTiles_MapHeight(void)
{
    return theNumOfTiles_MapHeight;
}

// Get the number of tiles for width of the map
int GameMap::getNumOfTiles_MapWidth(void)
{
    return theNumOfTiles_MapWidth;
}

Vector3 GameMap::getTileSize3D()
{
    return tileSizeXYZ;
}

bool GameMap::loadThoseLegends(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    GraphicsEntity *graphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
    if (file.is_open())
    {
        std::vector<std::string> theKeys;
        std::vector<std::string> theValues;
        std::string data = "";
        while (getline(file, data))
        {
            if (data == "" || data == "\n" || data == "\r")
                continue;
            std::string token;
            std::istringstream iss(data);
            if (theKeys.empty())
            {
                while (getline(iss, token, ','))
                {
                    if (token == "")
                        continue;
                    convertStringToUpperCaps(token);
                    theKeys.push_back(token);
                }
            }
            else {
                while (getline(iss, token, ','))
                {
                    theValues.push_back(token);
                }
                GameObject *theObject = new GameObject();
                char symbol = ' ';
                for (size_t num = 0; num < theKeys.size(); ++num)
                {
                    if (theKeys[num].find("NAME") != std::string::npos)
                    {
                        theObject->setName(theValues[num]);
                    }
                    else if (theKeys[num].find("CHARACTERSYMBOL") != std::string::npos)
                    {
                        symbol = theValues[num][0];
                    }
                    else if (theKeys[num].find("MESHID") != std::string::npos)
                    {
                        std::map<std::string, Mesh*>::iterator it = graphics->meshList.find(theValues[num]);
                        if (it != graphics->meshList.end())
                            theObject->SetMesh(*it->second);
                    }
                    else if (theKeys[num].find("SCALEX") != std::string::npos)
                    {
                        theObject->SetScale(Vector3(stof(theValues[num]), theObject->GetScale().y, theObject->GetScale().z));
                    }
                    else if (theKeys[num].find("SCALEY") != std::string::npos)
                    {
                        theObject->SetScale(Vector3(theObject->GetScale().x,stof(theValues[num]) , theObject->GetScale().z));
                    }
                    else if (theKeys[num].find("SCALEZ") != std::string::npos)
                    {
                        theObject->SetScale(Vector3(theObject->GetScale().x, theObject->GetScale().y, stof(theValues[num])));
                    }
                    else if (theKeys[num].find("ACTIVE") != std::string::npos)
                    {
                        if (theValues[num] == "0")
                            theObject->SetActive(false);
                        else
                            theObject->SetActive(true);
                    }
                }
                bunchOfLegends.insert(std::pair<unsigned char, GenericEntity*>(symbol, theObject));
                theValues.clear();
            }
        }

        return true;
    }
    return false;
}

void GameMap::convertStringToUpperCaps(std::string &theString)
{
    for (std::string::iterator it = theString.begin(), end = theString.end(); it != end; ++it) {
        (*it) = toupper(*it);
    }
}