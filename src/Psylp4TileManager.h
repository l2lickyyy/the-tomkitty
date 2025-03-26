#ifndef PSYLP4TILEMANAGER_H
#define PSYLP4TILEMANAGER_H

#include <cstdlib>
#include "BaseEngine.h"
#include "TileManager.h"
#include "DrawingSurface.h"

class Psylp4TileManager : public TileManager {
public:
	// constructor
	Psylp4TileManager()
		: TileManager(25, 25, 75, 75) {
	}

	virtual void virtDrawTileAt(BaseEngine* engine, DrawingSurface* surface, int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const override;
};

#endif PSYLP4TILEMANAGER_H