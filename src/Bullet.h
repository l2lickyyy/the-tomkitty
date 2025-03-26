/*
* Disabled!
* 
#ifndef BULLET_H
#define BULLET_H

#include <cmath>
#include "BaseEngine.h"
#include "DisplayableObject.h"
#include "UtilCollisionDetection.h"

class Bullet : public DisplayableObject {
public:
    Bullet(BaseEngine* engine, int x, int y, int velX, int velY, bool isTracer)
        : DisplayableObject(x, y, engine, 5, 5, true), m_bIsTracer(isTracer), velocityX(velX), velocityY(velY) {
        m_iCurrentScreenX = x;
        m_iCurrentScreenY = y;
        m_iDrawWidth = 5;
        m_iDrawHeight = 10;
        setVisible(true);

        int deltaX = velX;
        int deltaY = velY;
        double length = std::sqrt(deltaX * deltaX + deltaY * deltaY);
        int bulletSpeed = 10;
        if (length > 0) {
            velocityX = static_cast<int>((deltaX / length) * bulletSpeed);
            velocityY = static_cast<int>((deltaY / length) * bulletSpeed);
        }
        else {
            velocityX = bulletSpeed; // default to moving right if length is 0
            velocityY = 0;
        }
    }

    void virtDraw() override;
    void virtDoUpdate(int iCurrentTime) override;

private:
    bool m_bIsTracer;
    int velocityX;
    int velocityY;
};

#endif // BULLET_H
*/