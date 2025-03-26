#include "header.h"
#include "Psylp4TileManager.h"

void Psylp4TileManager::virtDrawTileAt(BaseEngine* engine, DrawingSurface* surface,
	int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const {

    int iMapValue = getMapValue(iMapX, iMapY);

    if (iMapValue == 1) { // draw obstacles
        surface->drawRectangle(
            iStartPositionScreenX, iStartPositionScreenY,
            iStartPositionScreenX + getTileWidth() - 1,
            iStartPositionScreenY + getTileHeight() - 1,
            0x000000);
    } else if (iMapValue == 2) { // turn tile green if user passes
        surface->drawRectangle(iStartPositionScreenX, iStartPositionScreenY,
            iStartPositionScreenX + getTileWidth() - 1,
            iStartPositionScreenY + getTileHeight() - 1,
            0x00FF00);
    } else if (iMapValue == 3) { // turn tile red if enemy passes
        surface->drawRectangle(iStartPositionScreenX, iStartPositionScreenY,
            iStartPositionScreenX + getTileWidth() - 1,
            iStartPositionScreenY + getTileHeight() - 1,
            0xFF0000);
    }
}