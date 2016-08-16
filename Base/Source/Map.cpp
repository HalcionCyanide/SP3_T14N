#include "Map.h"
#include <fstream>
#include <sstream>
#include "GraphicsEntity.h"
#include <assert.h>
#include "GameObject.h"
#include "Scene_System.h"

#ifndef MAX_NUM_POINTS
#define MAX_NUM_POINTS sizeof(points_4) / sizeof(int)
#endif

CMap::CMap(void)
    : theNumOfTiles_MapHeight(0)
    , theNumOfTiles_MapWidth(0)
    , theTileSize(0)
{
    theScreenMap.clear();
    tileSizeXYZ.SetZero();
    for (int num = 0; num < MAX_NUM_POINTS; ++num)
        points_4[num] = nullptr;
}

CMap::~CMap(void)
{
    theScreenMap.clear();
    for (int num = 0; num < MAX_NUM_POINTS; ++num)
    {
        if (points_4[num])
            delete points_4[num];
    }
}

//void CMap::Init(const int &theTileSize)
//{
//    this->theTileSize = theTileSize;
//}
//
bool CMap::LoadMap(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GenericEntity*> &theRenderingStuff)
{
    if (LoadFile(mapName, theHeightMap, terrainSize, theRenderingStuff) == true)
    {
        printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
        return true;
    }

    return false;
}

bool CMap::LoadFile(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GenericEntity*> &theRenderingStuff)
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
            if (token == "9999") {
                for (int num = 0; num < MAX_NUM_POINTS; ++num)
                {
                    if (points_4[num] == nullptr) {
                        points_4[num] = new unsigned(num_Column);
                        break;
                    }
                    else if (num == MAX_NUM_POINTS - 1)
                        assert(true == false);
                }
            }
            else if (token.find("H") != std::string::npos)
            {
                it->second.push_back(token[0]);
            }
            else if (token.find("G") != std::string::npos)
            {
                it->second.push_back(token[0]);
            }
            else if (token.find("T") != std::string::npos)
            {
                it->second.push_back(token[0]);
            }
            else if (token.find("C") != std::string::npos)
            {
                it->second.push_back(token[0]);
                GameObject *theCube = new GameObject();
                std::string theNumbers = token.substr(2);
                theCube->SetRotation(stof(theNumbers), Vector3(0, 1, 0));
                theCube->SetMesh(*theGraphics->meshList.find("cube")->second);
                theRenderingStuff.push_back(theCube);
            }
            else {
                it->second.push_back(' ');
            }
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

    }

    return true;
}

int CMap::GetTileSize(void)
{
    return theTileSize;
}

// Get the number of tiles for height of the map
int CMap::getNumOfTiles_MapHeight(void)
{
    return theNumOfTiles_MapHeight;
}

// Get the number of tiles for width of the map
int CMap::getNumOfTiles_MapWidth(void)
{
    return theNumOfTiles_MapWidth;
}

Vector3 CMap::getTileSize3D()
{
    return tileSizeXYZ;
}