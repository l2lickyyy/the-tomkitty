#include "header.h"
#include "Utility.h"
#include "foeObject.h"
#include "tomEngine.h"
#include "tomObject.h"
#include "tomTileManager.h"

void foeObject::virtDraw() {
    if (!isVisible()) return;

    currentSprite->renderImageWithMask(getEngine()->getForegroundSurface(),
        0, 0, m_iCurrentScreenX, m_iCurrentScreenY,
        currentSprite->getWidth(), currentSprite->getHeight(), 0x000000);
}

void foeObject::virtDoUpdate(int iCurrentTime) {
	if (!isVisible()) return;

    /*
	* Enemy object aims to close distance with the user.
    */

    tomObject* psylp4 = dynamic_cast<tomObject*>(getEngine()->getDisplayableObject(0));
    if (psylp4) {
        // movement functions
        int psylp4X = psylp4->getX();
        int psylp4Y = psylp4->getY();

        // offset handling
        static int offsetX = (rand() % 21) - 10;
        static int offsetY = (rand() % 21) - 10;

        m_iCurrentScreenX += ((m_iCurrentScreenX < psylp4X + offsetX) ? speedX : -speedX) + (rand() % 3 - 1);
        m_iCurrentScreenY += ((m_iCurrentScreenY < psylp4Y + offsetY) ? speedY : -speedY) + (rand() % 3 - 1);

        // update sprite
        currentSprite = (psylp4X + offsetX < m_iCurrentScreenX) ? &leftSprite : &rightSprite;

		// collision detection
        if (CollisionDetection::checkRectangles(
            m_iCurrentScreenX, m_iCurrentScreenX + getWidth(),
            m_iCurrentScreenY, m_iCurrentScreenY + getHeight(),
            psylp4->getX(), psylp4->getX() + psylp4->getWidth(),
            psylp4->getY(), psylp4->getY() + psylp4->getHeight())) {

            speedX = -speedX;
            speedY = -speedY;

            m_iCurrentScreenX += speedX * 2;
            m_iCurrentScreenY += speedY * 2;
        }

        // tile handling
        tomTileManager* tm = static_cast<tomEngine*>(getEngine())->getTileManager();
        int tileX = tm->getMapXForScreenX(m_iCurrentScreenX);
        int tileY = tm->getMapYForScreenY(m_iCurrentScreenY);
        if (tm->isValidTilePosition(tileX, tileY) && tm->getMapValue(tileX, tileY) == 1) {
            tm->setAndRedrawMapValueAt(tileX, tileY, 3, getEngine(), getEngine()->getBackgroundSurface()); // Change tile to gold

            // revert tile colour after 1 second
            SDL_AddTimer(1000, revertTile, new TileData{ tileX, tileY, tm, getEngine() });
        }

        // boundary checks for re-reversing
        if (m_iCurrentScreenX <= 0 || m_iCurrentScreenX + currentSprite->getWidth() >= getEngine()->getWindowWidth()) {
            speedX = -speedX;
        } if (m_iCurrentScreenY <= 0 || m_iCurrentScreenY + currentSprite->getHeight() >= getEngine()->getWindowHeight()) {
            speedY = -speedY;
        }

        redrawDisplay();
    }
}