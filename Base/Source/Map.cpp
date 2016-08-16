#include "Map.h"
#include <fstream>
#include <sstream>
#include "GraphicsEntity.h"


CMap::CMap(void)
    : theNumOfTiles_MapHeight(0)
    , theNumOfTiles_MapWidth(0)
    , theTileSize(0)
{
    theScreenMap.clear();
}

CMap::~CMap(void)
{
    theScreenMap.clear();
}

void CMap::Init(const int &theTileSize)
{
    this->theTileSize = theTileSize;
}

bool CMap::LoadMap(const std::string &mapName, std::vector<GenericEntity*> &theRenderingStuff)
{
    if (LoadFile(mapName, theRenderingStuff) == true)
    {
        printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
        return true;
    }

    return false;
}

bool CMap::LoadFile(const std::string &mapName, std::vector<GenericEntity*> &theRenderingStuff)
{
    int theLineCounter = 0;
    int theMaxNumOfColumns = 0;

    std::ifstream file(mapName.c_str());
    if (file.is_open())
    {
        int i = 0, k = 0;
        while (file.good())
        {
            std::string aLineOfText = "";
            getline(file, aLineOfText);

            if (aLineOfText == "")
                continue;

            // If this line is not a comment line, then process it
            if (!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
            {
                if (theLineCounter == 0)
                {
                    // This is the first line of the map data file
                    std::string token;
                    std::istringstream iss(aLineOfText);
                    while (getline(iss, token, ','))
                    {
                        // Count the number of columns
                        theMaxNumOfColumns = atoi(token.c_str());
                    }
                }
                else
                {
                    int theColumnCounter = 0;

                    std::string token;
                    std::istringstream iss(aLineOfText);
                    while (getline(iss, token, ','))
                    {
                        std::map<unsigned char, std::vector<unsigned char>>::iterator it = theScreenMap.find(theLineCounter);
                        if (it != theScreenMap.end()) {
                            it->second.push_back(atoi(token.c_str()));
                        }
                        else {
                            theScreenMap.insert(std::pair< unsigned char, std::vector<unsigned char> >(theLineCounter, std::vector<unsigned char>()));
                            theScreenMap.find(theLineCounter)->second.push_back(token.c_str()[0]);
                        }
                    }
                }
            }

            theLineCounter++;
        }
    }
    theNumOfTiles_MapWidth = theMaxNumOfColumns;
    theNumOfTiles_MapHeight = theLineCounter - 1;
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
