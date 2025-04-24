#pragma once
#include "TileManager.h"

class myTileManager : public TileManager {
public:
    myTileManager()
        : TileManager(20, 20, 15, 15) {
    }

    virtual void virtDrawTileAt(
        BaseEngine* pEngine,
        DrawingSurface* pSurface,
        int iMapX, int iMapY,
        int iStartPositionScreenX, int iStartPositionScreenY) const override;
};