#include "header.h"
#include "Utility.h"

Uint32 revertTile(Uint32 interval, void* param) {
    TileData* data = static_cast<TileData*>(param);
    if (data) {
        data->tm->setAndRedrawMapValueAt(data->x, data->y, 1, data->engine, data->engine->getBackgroundSurface());
        delete data; // free memory
    } return 0;
}