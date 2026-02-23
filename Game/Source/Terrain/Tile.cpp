#include "Tile.h"
#include <unordered_map>
#include "Core/FileIO.h"

std::string GetTileTypePath(const TileType tileType)
{
    switch (tileType)
    {
    case TileType::Grass:
        return Terra::FileIO::GetGameFile("Terrain\\GrassTiles.png");
    case TileType::Dirt:
        return Terra::FileIO::GetGameFile("Terrain\\DirtTiles.png");
    case TileType::Stone:
        return Terra::FileIO::GetGameFile("Terrain\\StoneTiles.png");
    case TileType::Air:
    case TileType::Water:
    case TileType::Bark:
    case TileType::Leaf:
    case TileType::Copper:
    case TileType::Gold:
    case TileType::Diamond:
        return Terra::FileIO::GetGameFile("Terrain\\BasicTiles.png");
    }
    return Terra::FileIO::GetGameFile("Terrain\\BasicTiles.png");
}



int GetTileMaskValue(const int bitmask)
{
    //pre-set list of numbers for certain tile mask outcomes.
    static const std::unordered_map<int, int> maskLookupList = {
        {2,1}, {8,2}, {10,3}, {11,4}, {16,5},
        {18,6}, {22,7}, {24,8}, {26,9}, {27,10},
        {30,11}, {31,12}, {64,13}, {66,14},
        {72,15}, {74,16}, {75,17}, {80,18},
        {82,19}, {86,20}, {88,21}, {90,22},
        {91,23}, {94,24}, {95,25}, {104,26},
        {106,27}, {107,28}, {120,29}, {122,30},
        {123,31}, {126,32}, {127,33}, {208,34},
        {210,35}, {214,36}, {216,37}, {218,38},
        {219,39}, {222,40}, {223,41}, {248,42},
        {250,43}, {251,44}, {254,45}, {255,46},
        {0,47}
    };

    //Check if bitmask is inside this list, if so replace it.
    
    const auto iterator = maskLookupList.find(bitmask);
    return (iterator != maskLookupList.end()) ? iterator->second : bitmask;
}
