#pragma once
#include "BaseEngine.h"
#include "tomTileManager.h"

struct TileData {
    int x, y;
    tomTileManager* tm;
    BaseEngine* engine;
};

// revert tile colour
Uint32 revertTile(Uint32 interval, void* param);