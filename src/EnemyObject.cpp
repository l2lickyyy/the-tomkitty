#include "header.h"
// #include "Bullet.h"
#include "Utility.h"
#include "EnemyObject.h"
#include "Psylp4Engine.h"
#include "Psylp4Object.h"
#include "Psylp4TileManager.h"

void EnemyObject::virtDraw() {
    if (!isVisible()) return;

    currentSprite->renderImageWithMask(getEngine()->getForegroundSurface(),
        0, 0, m_iCurrentScreenX, m_iCurrentScreenY,
        currentSprite->getWidth(), currentSprite->getHeight(), 0x000000);
}

void EnemyObject::virtDoUpdate(int iCurrentTime) {
	if (!isVisible()) return;

    /*
	* Enemy object aims to close distance with the user.
    * With an addition of 'offsets' for less predictable movement.
    */

    Psylp4Object* psylp4 = dynamic_cast<Psylp4Object*>(getEngine()->getDisplayableObject(0));
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
        Psylp4TileManager* tm = static_cast<Psylp4Engine*>(getEngine())->getTileManager();
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

        /*
        * bullet logic currently disabled due to vector issues
        * 
        // armament settings
        static int bulletCounter = 0;
        static int timeSinceLastShot = 0;
        const int RPM = 300; // rounds per minute
        const int fireRate = (60 * 1000) / RPM;
        const int pauseTime = 1000;

        // add pause in-between bursts of fire
        timeSinceLastShot += static_cast<Psylp4Engine*>(getEngine())->getLastFrametime();
        if (bulletCounter >= 5) {
            if (timeSinceLastShot < pauseTime) return; // Pause shooting
            bulletCounter = 0; // Reset counter after pause
            timeSinceLastShot = 0; // Reset timer
        }

        if (timeSinceLastShot >= fireRate) {
            timeSinceLastShot = 0;
            bool isTracer = (bulletCounter % 5 == 0);

            // calculate bullet direction
            int targetX = psylp4->getX();
            int targetY = psylp4->getY();
            float deltaX = static_cast<float>(targetX - m_iCurrentScreenX);
            float deltaY = static_cast<float>(targetY - m_iCurrentScreenY);
            float length = sqrt(deltaX * deltaX + deltaY * deltaY);

            if (length == 0) length = 1.0f; // prevent division by zero

            int velocityX = static_cast<int>(round((deltaX / length) * bulletSpeed));
            int velocityY = static_cast<int>(round((deltaY / length) * bulletSpeed));

            getEngine()->storeObjectInArray(static_cast<Psylp4Engine*>(getEngine())->getObjectCount(),
                new Bullet(getEngine(), m_iCurrentScreenX, m_iCurrentScreenY, velocityX, velocityY, isTracer));
            static_cast<Psylp4Engine*>(getEngine())->incrementObjectCount();
            bulletCounter++;
        }
        */

        redrawDisplay();
    }
}