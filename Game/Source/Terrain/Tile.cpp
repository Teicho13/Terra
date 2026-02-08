#include "Tile.h"
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
