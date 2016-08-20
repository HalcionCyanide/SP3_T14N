#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "GenericEntity.h"
#include <vector>
#include <map>
#include "Vector3.h"
#include "GameObject.h"
#include "..\\Systems\\BillboardManager.h"

class GameMap : public GenericEntity
{
public:
    GameMap(void);
    virtual ~GameMap(void);

    //virtual void Init(const int &theTileSize = 25);
    virtual bool LoadMap(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GameObject*> &theRenderingStuff, BillboardManager &theBBManager);
    int GetTileSize(void);

    int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
    int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

    std::map< unsigned, std::vector<unsigned char> > theScreenMap;
    Vector3 getTileSize3D();

protected:
    int theTileSize;

    int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
    int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

    virtual bool LoadFile(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GameObject*> &theRenderingStuff, BillboardManager &theBBManager);
    Vector3 tileSizeXYZ;

private:
    static std::map<unsigned char, GenericEntity*> bunchOfLegends;
    bool loadThoseLegends(const std::string &fileName);
    void convertStringToUpperCaps(std::string &theString);
};

#endif