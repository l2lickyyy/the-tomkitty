#ifndef UTILITY_H
#define UTILITY_H

#include "Psylp4TileManager.h"
#include "BaseEngine.h"
#include "SDL.h"

struct TileData {
    int x, y;
    Psylp4TileManager* tm;
    BaseEngine* engine;
};

// revert tile colour
Uint32 revertTile(Uint32 interval, void* param);

#endif // UTILITY_H