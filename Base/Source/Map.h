#ifndef MAP_H
#define MAP_H

#include "GenericEntity.h"
#include <vector>
#include <map>
#include "Vector3.h"

class CMap : public GenericEntity
{
public:
    CMap(void);
    virtual ~CMap(void);

    //virtual void Init(const int &theTileSize = 25);
    virtual bool LoadMap(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GenericEntity*> &theRenderingStuff);
    int GetTileSize(void);

    int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
    int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

    std::map< unsigned, std::vector<unsigned char> > theScreenMap;

    unsigned *points_4[4];

    Vector3 getTileSize3D();

protected:
    int theTileSize;

    int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
    int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

    virtual bool LoadFile(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GenericEntity*> &theRenderingStuff);
    Vector3 tileSizeXYZ;
};

#endif