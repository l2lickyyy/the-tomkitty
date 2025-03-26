#include "header.h"
#include "EnemyObject.h"
#include "Psylp4Engine.h"
#include "Psylp4Object.h"

// Background Function
void Psylp4Engine::virtSetupBackgroundBuffer() {
    // debug
    printf("setupBackgroundBuffer() running...\n");
    fflush(stdout);

	int height = getWindowHeight();
	int width = getWindowWidth();

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

        // draw background text
		drawBackgroundString(575, 300, "Hello, Psylp4!", 0xFFFFFF, getFont("Cornerstone Regular.ttf", 20));
    }

    // draw tiles
    if (gameState == STATE_GAME) {
        for (int i = 0; i < tm.getMapWidth(); i++) {
            for (int j = 0; j < tm.getMapHeight(); j++) {
                // randomly set some tiles as obstacles
                tm.setMapValue(i, j, (rand() % 10 == 0) ? 1 : 0); // 10% chance of obstacle
            }
        } tm.drawAllTiles(this, getBackgroundSurface());
    }
}

// Main Redraw Function
void Psylp4Engine::virtMainLoopDoBeforeUpdate() {
    if (counter >= 1000) {
        counter = 0;
    }

    // text breathing effect and counter
    if (counter % 4 == 0) { // adjust speed
        if (increasing) {
            breathingEffect += 15;
            if (breathingEffect >= 255) increasing = false;
        } else {
            breathingEffect -= 15;
            if (breathingEffect <= 80) increasing = true;
        } redrawDisplay();
    } counter++;

    if (gameState == STATE_GAME) {
        redrawDisplay(); // force redraw to update tile colors
    }

    // destroy marked objects
    destroyObject();
}

// Dynamic Text Function
void Psylp4Engine::virtDrawStringsOnTop() {
    // breathing effects
    int colour = (breathingEffect << 16) | (breathingEffect << 8) | breathingEffect;
    char text[50];

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
}

// Keyboard Function
void Psylp4Engine::virtKeyDown(int iKeyCode) {
    switch (iKeyCode) {
    case SDLK_ESCAPE:
        if (gameState == STATE_MENU) {
            setExitWithCode(0); // exit program
		} else if (gameState == STATE_CONTROL_SELECT) {
			gameState = STATE_MENU; // return to menu
			virtSetupBackgroundBuffer();
			redrawDisplay();
        } else if (gameState == STATE_GAME) {
			gameState = STATE_PAUSE; // pause game
        } break;

    case SDLK_RETURN:
        if (gameState == STATE_MENU) {
            gameState = STATE_CONTROL_SELECT; // control selection
            virtSetupBackgroundBuffer();
            redrawDisplay();
        } else if (gameState == STATE_PAUSE) {
			gameState = STATE_GAME; // resume game
        } break;

    case SDLK_SPACE:
        if (gameState == STATE_CONTROL_SELECT) {
			useKeyboard = true; // enable keyboard control
            gameState = STATE_GAME;
            virtSetupBackgroundBuffer();
            redrawDisplay();
            virtInitialiseObjects();
        } break;

    case SDLK_r:
        if (gameState == STATE_PAUSE) {
            gameState = STATE_MENU;
            drawableObjectsChanged();
            destroyOldObjects(true); // clear objects
            virtSetupBackgroundBuffer();
            redrawDisplay();
        } break;

    default:
        break;
    }
}

// Mouse Function
void Psylp4Engine::virtMouseDown(int iButton, int iX, int iY) {
    if (iButton == SDL_BUTTON_LEFT) {
        if (gameState == STATE_CONTROL_SELECT) {
			useKeyboard = false; // disable keyboard control when mouse enabled
            gameState = STATE_GAME;
            virtSetupBackgroundBuffer();
            redrawDisplay();
            virtInitialiseObjects();
        }
    }
}

// Object Function
int Psylp4Engine::virtInitialiseObjects() {
	if (gameState != STATE_GAME) return 0; // do nothing if game hasn't started

    // debug
    printf("virtInitialiseObjects() running...\n");
    fflush(stdout);

    // clear previous objects
    drawableObjectsChanged();
    destroyOldObjects(true);

    // add object
    DisplayableObject* objectsToCreate[] = {
        new Psylp4Object(this),
        new EnemyObject(this, 100, 100)
    };

    int numObjects = sizeof(objectsToCreate) / sizeof(objectsToCreate[0]);
    createObjectArray(numObjects+99);
    for (int i = 0; i < numObjects; i++) {
        storeObjectInArray(i, objectsToCreate[i]);
        objectCount++;
    }

    setAllObjectsVisible(true);

	// disable mouse control if keyboard control enabled
    if (useKeyboard) {
        DisplayableObject* obj = getDisplayableObject(0);
        if (obj && dynamic_cast<Psylp4Object*>(obj)) {
            static_cast<Psylp4Object*>(obj)->setMouseControl(false);
        }
    }

    return 0;
}

/*
* HELPER FUNCTIONS
*/

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

// mark object to destroy
void Psylp4Engine::markToDestroy(DisplayableObject* obj){
    objectToDestroy.push_back(obj);
}

// destroy object
void Psylp4Engine::destroyObject() {
    for (DisplayableObject* obj : objectToDestroy) {
        if (obj) { // ensure object is valid
            removeDisplayableObject(obj);
            delete obj; // free memory
        }
    } objectToDestroy.clear();
}

// get last object frametime
int Psylp4Engine::getLastFrametime() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = now - lastFrametime;
    lastFrametime = now;
    return static_cast<int>(elapsed.count());
}