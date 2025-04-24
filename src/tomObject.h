#pragma once
#include "SimpleImage.h"
#include "ImageManager.h"
#include "tomEngine.h"
#include "DisplayableObject.h"

class tomObject : public DisplayableObject {  
public:
    tomObject(BaseEngine* pEngine)
        : DisplayableObject(pEngine, 100, 100, true), speed(7), 
        idleSprite(ImageManager::loadImage("images/sprite.png", true)),
        leftSprite(ImageManager::loadImage("images/spriteL.png", true)),
        rightSprite(ImageManager::loadImage("images/spriteR.png", true)),
        currentSprite(&idleSprite), mouseControl(true) {
        m_iCurrentScreenX = 500;
        m_iCurrentScreenY = 30;
        setVisible(true);

        // debug
        printf("Psylp4Object constructor called!\n");  
        fflush(stdout);

        printf("Object created at (%d, %d)\n", m_iCurrentScreenX, m_iCurrentScreenY);
        fflush(stdout);
    }  

    void virtDraw() override;  
    void virtDoUpdate(int iCurrentTime) override;

    // helper functions
    void setMouseControl(bool enabled) { mouseControl = enabled; }
	int getX() const { return m_iCurrentScreenX; }
	int getY() const { return m_iCurrentScreenY; }
    int getWidth() const { return currentSprite->getWidth(); }
    int getHeight() const { return currentSprite->getHeight(); }

private:
    int speed;  
    SimpleImage idleSprite;
    SimpleImage leftSprite;
    SimpleImage rightSprite;
    SimpleImage* currentSprite;
    bool mouseControl;
};