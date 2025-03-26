# Project Documentation

## 1. BaseEngine Sub-Class

### Psylp4Engine Class

- **Purpose**: `Psylp4Engine` is a sub-class of `BaseEngine` designed to manage the game loop, user input, and object interactions.
- **Background**: The background is set up with a gradient effect, differentiating it from the demos. It uses shape drawing functions to create a visually distinct background.
- **Implementation**:
  - The background is initialized in the `virtSetupBackgroundBuffer()` function, which draws a gradient and additional shapes to ensure a unique appearance.

```cpp
void Psylp4Engine::virtSetupBackgroundBuffer() {

  (...existing code...)

  // draw gradient background
  getBackgroundSurface()->mySDLLockSurface(); // to prevent sdl locking error
  for (int y = 0; y < height; y++) {
    int blue = 255 - (y * 128 / height);
    int green = (y * 255 / height);
    int color = (green << 8) | blue;
    for (int x = 0; x < width; x++) {
        setBackgroundPixel(x, y, color);
    }
  }
  getBackgroundSurface()->mySDLUnlockSurface(); // to prevent sdl locking error

  (...existing code...)
}
```

## 2. Drawing Functions

### Background Drawing

- **Shapes**: The background features a gradient created using rectangles, providing a visually appealing backdrop.
- **Image**: An image is loaded from a file and rendered onto the background, ensuring it is not obliterated by moving objects.

```cpp
void Psylp4Engine::virtSetupBackgroundBuffer() {

  (...existing code...)

  // draw background in menu
  if (gameState == STATE_MENU) {
    // draw background shape
    drawBackgroundRectangle(55, 60, 200, 50, 0xFF0000);

    // draw background image
    if (!backgroundImage.exists()) { // if image NOT exist, load
        backgroundImage = ImageManager::loadImage("images/psylp4-E.png", true); // load image
    } if (backgroundImage.exists()) { // if image exists, render
        backgroundImage.renderImageWithMask(getBackgroundSurface(), 0, 0, 410, 50,
        backgroundImage.getWidth(), backgroundImage.getHeight(), 0x000000);
    }

  (...existing code...)
}
```

## 3. Text on Background

### Static Text

- **Implementation**: Text is drawn onto the background using the `drawBackgroundString()` function.
- **Persistence**: The text remains visible as objects move over it, showcasing the redrawing mechanism where the background is drawn first, followed by moving objects.

```cpp
void Psylp4Engine::virtSetupBackgroundBuffer() {

  (...existing code...)

  // draw background text
  drawBackgroundString(575, 300, "Hello, Psylp4!", 0xFFFFFF, getFont("Cornerstone Regular.ttf", 20));
  }

  (...existing code...)
}
```

## 4. Changing Text on Foreground

### Dynamic Text

- **Implementation**: Dynamic text, such as a counter or score, is drawn onto the foreground using the `drawTextWithOutline()` function.

```cpp
// draw text outline
void Psylp4Engine::drawTextWithOutline(int x, int y, const char* text, int colour, DrawingSurface* surface, Font* font) {
    // draw outline
    drawForegroundString(x - 2, y, text, 0x000000, font);
    drawForegroundString(x + 2, y, text, 0x000000, font);
    drawForegroundString(x, y - 2, text, 0x000000, font);
    drawForegroundString(x, y + 2, text, 0x000000, font);

    // draw main text
    drawForegroundString(x, y, text, colour, font);
}
```

- **Updates**: The text changes over time, depends on the state of the game, and is redrawn appropriately to remove old text.

```cpp
// display counter
snprintf(text, sizeof(text), "Counter: %d", counter);
drawTextWithOutline(1100, 10, text, 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 20));

if (gameState == STATE_MENU) {
    drawTextWithOutline(25, 20, "Esc to Exit Game", 0xFF0000, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 20));
    drawTextWithOutline(510, 600, "Press Enter!", colour, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 40));
} else if (gameState == STATE_PAUSE) {
    // darken screen
    getForegroundSurface()->mySDLLockSurface();
    getForegroundSurface()->drawRectangle(0, 0, getWindowWidth(), getWindowHeight(), 0x1F000000);
    getForegroundSurface()->mySDLUnlockSurface();
    drawTextWithOutline(530, 250, "PAUSED", 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 50));
    drawTextWithOutline(495, 350, "Enter to Resume", 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 30));
    drawTextWithOutline(460, 435, "R to Return to Menu", 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 30));
} else if (gameState == STATE_CONTROL_SELECT) {
    drawTextWithOutline(520, 300, "Choose Controls!", 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 30));
    drawTextWithOutline(430, 350, "SPACEBAR to use Keyboard", 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 30));
    drawTextWithOutline(515, 385, "LMB to use Mouse", 0xFFFFFF, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 30));
	drawTextWithOutline(25, 20, "Esc to Return", 0xFF0000, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 20));
} else if (gameState == STATE_GAME) {
    drawTextWithOutline(25, 20, "Esc to Pause", 0xFF0000, getForegroundSurface(), getFont("Cornerstone Regular.ttf", 20));
}
```

## 5. User-Controlled Moving Object

### Psylp4Object Class

- **Purpose**: A sub-class of `DisplayableObject` that represents the user-controlled object in the game.

```cpp
void Psylp4Object::virtDraw() {
    if (!isVisible()) return;

    currentSprite->renderImageWithMask(getEngine()->getForegroundSurface(),
        0, 0, m_iCurrentScreenX, m_iCurrentScreenY,
        currentSprite->getWidth(), currentSprite->getHeight(), 0x000000);
}
```

- **Control**: The object can be moved using either the keyboard (WASD or arrow keys) or the mouse.
- **Interaction**: The object interacts with tiles and other objects, changing tiles as it passes over them and handling collisions with the enemy object.

```cpp
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
```

## 6. Keyboard and Mouse Input

### Input Handling

- **Keyboard**: The user-controlled object responds to keyboard input for movement and other actions, such as incrementing a counter.
- **Mouse**: The object can also be controlled using the mouse, following the cursor's position. Mouse input is handled to ensure both control methods are functional.


```cpp
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
    }
}
```

## 7. Automated Moving Object

### EnemyObject Class

- **Purpose**: A sub-class of `DisplayableObject` that represents an automated moving object in the game.

```cpp
void EnemyObject::virtDraw() {
    if (!isVisible()) return;

    currentSprite->renderImageWithMask(getEngine()->getForegroundSurface(),
        0, 0, m_iCurrentScreenX, m_iCurrentScreenY,
        currentSprite->getWidth(), currentSprite->getHeight(), 0x000000);
}
```

- **Movement**: The enemy object follows the user-controlled object with some random offsets, providing unpredictable movement.

```cpp
static int offsetX = (rand() % 21) - 10;
static int offsetY = (rand() % 21) - 10;

int psylp4X = psylp4->getX();
int psylp4Y = psylp4->getY();

m_iCurrentScreenX += ((m_iCurrentScreenX < psylp4X + offsetX) ? speedX : -speedX) + (rand() % 3 - 1);
m_iCurrentScreenY += ((m_iCurrentScreenY < psylp4Y + offsetY) ? speedY : -speedY) + (rand() % 3 - 1);
```

- **Interaction**: The enemy object interacts with tiles and the user-controlled object, changing tiles as it passes over them and handling collisions.

```cpp
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
```

## 8. Collision Detection

### Interaction Between Objects

- **Implementation**: Collision detection is implemented using rectangle-rectangle interaction checks.
- **Response**: When the enemy object collides with the user-controlled object, it will respond by changing direction or position, and when it hits screen boundaries, it will reverse back to following user-controlled object, visibly altering the game state.

```cpp
// collision detection for psylp4 object
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

// collision detection for enemy object
Psylp4Object* psylp4 = dynamic_cast<Psylp4Object*>(getEngine()->getDisplayableObject(0));
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

// direction reversal
if (m_iCurrentScreenX <= 0 || m_iCurrentScreenX + currentSprite->getWidth() >= getEngine()->getWindowWidth()) {
    speedX = -speedX;
} if (m_iCurrentScreenY <= 0 || m_iCurrentScreenY + currentSprite->getHeight() >= getEngine()->getWindowHeight()) {
    speedY = -speedY;
}
```

## 9. Sub-class of TileManager

### Psylp4TileManager Class

- **Purpose**: A sub-class of `TileManager` that manages the game's tile map with unique behavior.
- **Drawing**: Tiles are drawn to the background and interact with moving objects, changing color when passed over by the user-controlled or enemy object.
- **Visibility**: The tile map is visible to the user, with tiles changing colour based on interactions, demonstrating the custom behavior of the tile manager.

```cpp
#include "header.h"
#include "Psylp4TileManager.h"

void Psylp4TileManager::virtDrawTileAt(BaseEngine* engine, DrawingSurface* surface,
	int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const {

    int iMapValue = getMapValue(iMapX, iMapY);

    if (iMapValue == 1) { // draw obstacles
        surface->drawRectangle(
            iStartPositionScreenX, iStartPositionScreenY,
            iStartPositionScreenX + getTileWidth() - 1,
            iStartPositionScreenY + getTileHeight() - 1,
            0x000000);
    } else if (iMapValue == 2) { // turn tile green if user passes
        surface->drawRectangle(iStartPositionScreenX, iStartPositionScreenY,
            iStartPositionScreenX + getTileWidth() - 1,
            iStartPositionScreenY + getTileHeight() - 1,
            0x00FF00);
    } else if (iMapValue == 3) { // turn tile red if enemy passes
        surface->drawRectangle(iStartPositionScreenX, iStartPositionScreenY,
            iStartPositionScreenX + getTileWidth() - 1,
            iStartPositionScreenY + getTileHeight() - 1,
            0xFF0000);
    }
}
```

## 10. Object-Tile Interaction

### Tile Interaction

- **Implementation**: Moving objects interact with the tile map, changing specific tiles as they pass over them.

```cpp
// tile handling for psylp4 object
Psylp4TileManager* tm = static_cast<Psylp4Engine*>(getEngine())->getTileManager();
int tileX = tm->getMapXForScreenX(m_iCurrentScreenX);
int tileY = tm->getMapYForScreenY(m_iCurrentScreenY);
if (tm->isValidTilePosition(tileX, tileY) && tm->getMapValue(tileX, tileY) == 1) {
    tm->setAndRedrawMapValueAt(tileX, tileY, 3, getEngine(), getEngine()->getBackgroundSurface()); // Change tile to gold

    // revert tile colour after 1 second
    SDL_AddTimer(1000, revertTile, new TileData{ tileX, tileY, tm, getEngine() });
}

// tile handling for enemy object
Psylp4TileManager* tm = static_cast<Psylp4Engine*>(getEngine())->getTileManager();
int tileX = tm->getMapXForScreenX(newX);
int tileY = tm->getMapYForScreenY(newY);
if (tm->isValidTilePosition(tileX, tileY) && tm->getMapValue(tileX, tileY) == 1) {
    tm->setAndRedrawMapValueAt(tileX, tileY, 2, getEngine(), getEngine()->getBackgroundSurface()); // Change tile to gold

    // revert tile colour after 1 second
    SDL_AddTimer(1000, revertTile, new TileData{ tileX, tileY, tm, getEngine() });
}
```

- **Visibility**: Tiles change color and are redrawn correctly, ensuring the user sees the changes in real-time. This interaction is managed by checking the object's position relative to the tile map.

```cpp
if (iMapValue == 1) { // draw obstacles
    surface->drawRectangle(
        iStartPositionScreenX, iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1,
        0x000000);
} else if (iMapValue == 2) { // turn tile green if user passes
    surface->drawRectangle(iStartPositionScreenX, iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1,
        0x00FF00);
} else if (iMapValue == 3) { // turn tile red if enemy passes
    surface->drawRectangle(iStartPositionScreenX, iStartPositionScreenY,
        iStartPositionScreenX + getTileWidth() - 1,
        iStartPositionScreenY + getTileHeight() - 1,
        0xFF0000);
}
```