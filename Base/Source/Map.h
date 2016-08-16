#ifndef MAP_H
#define MAP_H

#include "GenericEntity.h"
#include <vector>
#include <map>

class CMap : public GenericEntity
{
public:
    CMap(void);
    virtual ~CMap(void);

    virtual void Init(const int &theTileSize = 25);
    virtual bool LoadMap(const std::string &mapName, std::vector<GenericEntity*> &theRenderingStuff);
    int GetTileSize(void);

    int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
    int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

    //vector<vector<int> > theScreenMap;
    std::map<unsigned char, std::vector<unsigned char>> theScreenMap;

    int getScreenWidth();
    int getScreenHeight();

protected:
    int theTileSize;

    int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
    int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

    virtual bool LoadFile(const std::string &mapName, std::vector<GenericEntity*> &theRenderingStuff);
};

#endif