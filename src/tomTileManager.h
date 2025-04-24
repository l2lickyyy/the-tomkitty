#pragma once
#include "BaseEngine.h"
#include "TileManager.h"
#include "DrawingSurface.h"

class tomTileManager : public TileManager {
public:
	// constructor
	tomTileManager()
		: TileManager(25, 25, 75, 75) {
	}

	virtual void virtDrawTileAt(BaseEngine* engine, DrawingSurface* surface, int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const override;
};