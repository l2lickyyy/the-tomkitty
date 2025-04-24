#pragma once
#include "DisplayableObject.h"
class objectB :
    public DisplayableObject {
public:
    objectB(BaseEngine* pEngine)
        : DisplayableObject(100, 200, pEngine, 100, 200, true) {
    }

    void virtDraw();
    void virtDoUpdate(int iCurrentTime);
};