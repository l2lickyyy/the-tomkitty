#include "BaseEngine.h"
#include "myTileManager.h"

class demoA :
    public BaseEngine {

public:
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY);
    void virtKeyDown(int iKeyCode);
    int virtInitialiseObjects();

protected:
    myTileManager tm;
};