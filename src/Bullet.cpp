/*
* Disabled!
* 
#include "header.h"
#include "Bullet.h"
#include "Psylp4Engine.h"
#include "Psylp4Object.h"

void Bullet::virtDraw() {
    if (!isVisible()) return;

    getEngine()->drawForegroundRectangle(
        m_iCurrentScreenX, m_iCurrentScreenY,
        m_iCurrentScreenX + m_iDrawWidth,
        m_iCurrentScreenY + m_iDrawHeight,
        m_bIsTracer ? 0xFF0000 : 0xFFFFFF);
}

void Bullet::virtDoUpdate(int iCurrentTime) {
    if (!isVisible()) return;

    // move bullet
    m_iCurrentScreenX += velocityX;
    m_iCurrentScreenY += velocityY;

    // destroy if out of bounds
    if (m_iCurrentScreenX < 0 || m_iCurrentScreenX > getEngine()->getWindowWidth() ||
        m_iCurrentScreenY < 0 || m_iCurrentScreenY > getEngine()->getWindowHeight()) {
        static_cast<Psylp4Engine*>(getEngine())->markToDestroy(this);
        return;
    }

    // collision detection
    Psylp4Object* psylp4 = dynamic_cast<Psylp4Object*>(getEngine()->getDisplayableObject(0));
    if (psylp4 && CollisionDetection::checkRectangles(
        m_iCurrentScreenX, m_iCurrentScreenX + m_iDrawWidth,
        m_iCurrentScreenY, m_iCurrentScreenY + m_iDrawHeight,
        psylp4->getX(), psylp4->getX() + psylp4->getWidth(),
        psylp4->getY(), psylp4->getY() + psylp4->getHeight())) {
        static_cast<Psylp4Engine*>(getEngine())->markToDestroy(this);
        return;
    }
}
*/