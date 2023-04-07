#include "Map.h"
#include "Game.h"
#include <fstream>

using namespace std;

Map::Map()
{
}

void Map::LoadMap(string path, int sizeX, int sizeY)
{
    char tile;
    fstream mapFile;
    mapFile.open(path);

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            Game::AddTile(atoi(&tile), x * 32, y * 32);
            mapFile.ignore();
        }
    }

    mapFile.close();
}

Map::~Map()
{
}

