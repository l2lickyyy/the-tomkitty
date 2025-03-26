#ifndef PSYLP4ENGINE_H
#define PSYLP4ENGINE_H

#include <chrono>
#include <vector>
#include "BaseEngine.h"
#include "SimpleImage.h"#
#include "Psylp4TileManager.h"

enum GameState { STATE_MENU, STATE_CONTROL_SELECT, STATE_GAME, STATE_PAUSE };

class Psylp4Engine : public BaseEngine {  
public:  
  // Add these lines at the beginning of the constructor  
  Psylp4Engine::Psylp4Engine() : gameState(STATE_MENU), counter(0), breathingEffect(150), objectCount(0),
      increasing(true), useKeyboard(false), lastFrametime(std::chrono::steady_clock::now()) {
      #ifdef DEBUG
          std::cout << "Psylp4Engine constructor called!" << std::endl;
      #endif
  }

  virtual ~Psylp4Engine() {}  

  void virtSetupBackgroundBuffer() override;
  void virtDrawStringsOnTop() override;
  void virtMainLoopDoBeforeUpdate() override;
  void virtKeyDown(int iKeyCode) override;
  void virtMouseDown(int iButton, int iX, int iY) override;
  int virtInitialiseObjects() override;

  // helper functions
  GameState getGameState() const { return gameState; }
  Psylp4TileManager* getTileManager() { return &tm; }

  void drawTextWithOutline(int x, int y, const char* text, int colour, DrawingSurface* surface, Font* font);
  void markToDestroy(DisplayableObject* obj);
  void destroyObject();

  int getObjectCount() const { return objectCount; }
  int getLastFrametime();

private:
   SimpleImage backgroundImage;
   Psylp4TileManager tm;
   GameState gameState;

   int counter;
   int breathingEffect;
   int objectCount;

   bool increasing;
   bool useKeyboard;

   std::chrono::steady_clock::time_point lastFrametime;
   std::vector<DisplayableObject*> objectToDestroy;
};

#endif // PSYLP4ENGINE_H