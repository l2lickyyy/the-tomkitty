#include "header.h"
#include "SDL.h"
#include "UtilCollisionDetection.h"
#include "Utility.h"
#include "EnemyObject.h"
#include "Psylp4Object.h"
#include "Psylp4TileManager.h"

void Psylp4Object::virtDraw() {
    if (!isVisible()) return;

    currentSprite->renderImageWithMask(getEngine()->getForegroundSurface(),
        0, 0, m_iCurrentScreenX, m_iCurrentScreenY,
        currentSprite->getWidth(), currentSprite->getHeight(), 0x000000);
}

void Psylp4Object::virtDoUpdate(int iCurrentTime) {
	if (static_cast<Psylp4Engine*>(getEngine())->getGameState() == STATE_PAUSE) return;

    int newX = m_iCurrentScreenX;
    int newY = m_iCurrentScreenY;
    
    // keyboard handling
    if (!mouseControl) {
        if (getEngine()->isKeyPressed(SDLK_LEFT) || getEngine()->isKeyPressed(SDLK_a)) {
            newX -= speed;
        } if (getEngine()->isKeyPressed(SDLK_RIGHT) || getEngine()->isKeyPressed(SDLK_d)) {
            newX += speed;
        } if (getEngine()->isKeyPressed(SDLK_UP) || getEngine()->isKeyPressed(SDLK_w)) {
            newY -= speed;
        } if (getEngine()->isKeyPressed(SDLK_DOWN) || getEngine()->isKeyPressed(SDLK_s)) {
            newY += speed;
        }

        // uodate sprite based on movement
        if (getEngine()->isKeyPressed(SDLK_LEFT) || getEngine()->isKeyPressed(SDLK_a)) {
            currentSprite = &leftSprite;
        } else if (getEngine()->isKeyPressed(SDLK_RIGHT) || getEngine()->isKeyPressed(SDLK_d)) {
            currentSprite = &rightSprite;
		} else { // ensure idle sprite is displayed when not moving
            currentSprite = &idleSprite;
        }
    } else {
        int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX != m_iCurrentScreenX || mouseY != m_iCurrentScreenY) {
            // follow mouse cursor
            if (mouseX < m_iCurrentScreenX) newX -= speed;
            if (mouseX > m_iCurrentScreenX) newX += speed;
            if (mouseY < m_iCurrentScreenY) newY -= speed;
            if (mouseY > m_iCurrentScreenY) newY += speed;

            // update sprite based on mouse movement
            if (mouseX < m_iCurrentScreenX) {
                currentSprite = &leftSprite;
            } else if (mouseX > m_iCurrentScreenX) {
                currentSprite = &rightSprite;
            } else {
                currentSprite = &idleSprite;
            }
        }
    }

    // tile handling
    Psylp4TileManager* tm = static_cast<Psylp4Engine*>(getEngine())->getTileManager();
    int tileX = tm->getMapXForScreenX(newX);
    int tileY = tm->getMapYForScreenY(newY);
    if (tm->isValidTilePosition(tileX, tileY) && tm->getMapValue(tileX, tileY) == 1) {
        tm->setAndRedrawMapValueAt(tileX, tileY, 2, getEngine(), getEngine()->getBackgroundSurface()); // Change tile to gold

        // revert tile colour after 1 second
        SDL_AddTimer(1000, revertTile, new TileData{ tileX, tileY, tm, getEngine() });
    }

    // collision detection
    EnemyObject* enemy = dynamic_cast<EnemyObject*>(getEngine()->getDisplayableObject(1));
    if (enemy && CollisionDetection::checkRectangles(
        newX, newX + getWidth(),
        newY, newY + getHeight(),
        enemy->getX(), enemy->getX() + enemy->getWidth(),
        enemy->getY(), enemy->getY() + enemy->getHeight())) {

        // prevent object overlapping
        if (newX < enemy->getX()) newX -= 10; else newX += 10;
        if (newY < enemy->getY()) newY -= 10; else newY += 10;
    }

    // restrict movement bounds
    m_iCurrentScreenX = std::max(0, std::min(newX, getEngine()->getWindowWidth() - m_iDrawWidth));
    m_iCurrentScreenY = std::max(0, std::min(newY, getEngine()->getWindowHeight() - m_iDrawHeight));

    // mark for redrawing
    redrawDisplay();
}