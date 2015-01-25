#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "constants.h"

bool running = true;
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;

bool outsideWindow(GameEntity* gameEntity);

int main(int argc, char** argv) {
  // Initialize
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL couldn't initialize: " << SDL_GetError() << std::endl;
    return 0;
  }

  window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    std::cout << "Window couldn't be created: " << SDL_GetError() << std::endl;
    return 0;
  }
  surface = SDL_GetWindowSurface(window);

  Player* player = new Player;

  std::vector<GameEntity*> gameEntities;
  gameEntities.push_back(player);
  for (int i = 0; i < 8; ++i) {
    gameEntities.push_back(new Alien(30 + 60 * i, 30));
  }

  Uint32 lastTime = SDL_GetTicks();

  // Main loop
  while (running) {
    // Timing
    Uint32 currentTime = SDL_GetTicks();
    Uint32 delta = currentTime - lastTime;
    lastTime = currentTime;

    std::cout << "size: " << gameEntities.size() << std::endl << std::flush;
    // Clear, update, and draw
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
    for (auto &gameEntity : gameEntities) {
      gameEntity->update(delta);
      SDL_BlitScaled(gameEntity->bitmap, NULL, surface, &(gameEntity->rect));
    }
    SDL_UpdateWindowSurface(window);

    // Remove entities outside of screen. Todo: Should delete as well.
    gameEntities.erase(std::remove_if(std::begin(gameEntities), std::end(gameEntities), outsideWindow), std::end(gameEntities));

    // Handle events
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        running = false;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_ESCAPE:
            running = false;
            break;
          case SDLK_SPACE: 
            {
              int centerX = player->x + player->rect.w / 2;
              gameEntities.push_back(new Bullet(centerX, player->y));
              break;
            }
          default:
            break;
        }
      }
    }

    // Handle keystate
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) {
      player->velocity = -0.2f;
    } else if (state[SDL_SCANCODE_RIGHT]) {
      player->velocity = 0.2f;
    } else {
      player->velocity = 0;
    }
  }

  delete player;
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

bool outsideWindow(GameEntity* gameEntity) {
  if (gameEntity->x < 0) {
    return true;
  }
  if (gameEntity->x + gameEntity->rect.w > SCREEN_WIDTH) {
    return true;
  }
  if (gameEntity->y < 0) {
    return true;
  }
  if (gameEntity->y + gameEntity->rect.h > SCREEN_HEIGHT) {
    return true;
  }
  return false;
}
