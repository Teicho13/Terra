#pragma once
#include <string>

enum class TileType
{
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Water,
    Bark,
    Leaf,
    Copper,
    Gold,
    Diamond
};

struct TileData
{
    TileType tiletype = TileType::Air;
    int tilemask = 0;
};

std::string GetTileTypePath(TileType tileType);

int GetTileMaskValue(int bitmask);
