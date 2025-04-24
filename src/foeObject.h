#pragma once
#include "BaseEngine.h"
#include "SimpleImage.h"
#include "ImageManager.h"
#include "DisplayableObject.h"
#include "UtilCollisionDetection.h"

class foeObject : public DisplayableObject {
public:
    foeObject(BaseEngine* engine, int startX, int startY)
        : DisplayableObject(engine, startX, startY, true), speedX(3), speedY(3) {
        centreSprite = ImageManager::loadImage("images/enemySprite.png", true);
        leftSprite = ImageManager::loadImage("images/enemySpriteL.png", true);
        rightSprite = ImageManager::loadImage("images/enemySpriteR.png", true);
        currentSprite = &centreSprite;
        m_iCurrentScreenX = rand() % (engine->getWindowWidth() - centreSprite.getWidth());
        m_iCurrentScreenY = rand() % (engine->getWindowHeight() / 3);
        setVisible(true);
    }

    void virtDraw() override;
    void virtDoUpdate(int iCurrentTime) override;

    // helper functions
    int getX() const { return m_iCurrentScreenX; }
    int getY() const { return m_iCurrentScreenY; }
    int getWidth() const { return currentSprite->getWidth(); }
    int getHeight() const { return currentSprite->getHeight(); }

private:
    SimpleImage centreSprite;
    SimpleImage leftSprite;
    SimpleImage rightSprite;
    SimpleImage* currentSprite;
    int speedX;
    int speedY;
};