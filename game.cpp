#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "GameEntity.h"
#include "Player.h"
#include "Alien.h"
#include "Bullet.h"
#include "constants.h"

bool running = true;
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;

bool outsideWindow(GameEntity* gameEntity);
bool collision(GameEntity* geA, GameEntity* geB);

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
  std::vector<Alien*> aliens;
  std::vector<Bullet*> bullets;

  gameEntities.push_back(player);

  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 5; ++j) {
      Alien *a = new Alien(ENEMY_MARGIN + 55 * i, 0, j);
      gameEntities.push_back(a);
      aliens.push_back(a);
    }
  }


  Uint32 lastTime = SDL_GetTicks();
  Uint32 lastShot = 0;

  // Main loop
  while (running) {
    // Timing
    Uint32 currentTime = SDL_GetTicks();
    Uint32 delta = currentTime - lastTime;
    lastTime = currentTime;

    /* std::cout << "size: " << gameEntities.size() << std::endl << std::flush; */
    // Clear, update, and draw
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
    for (auto &gameEntity : gameEntities) {
      gameEntity->update(delta);
      SDL_BlitScaled(gameEntity->bitmap, gameEntity->srcrect, surface, &(gameEntity->dstrect));
    }
    SDL_UpdateWindowSurface(window);

    // Check for collision between bullets and aliens. Also destroys bullets
    // outside of the window.
    // Todo: Clean this up.
    for (int i = 0; i < bullets.size(); ++i) {
      for (int j = 0; j < aliens.size(); ++j) {
        if (collision(bullets[i], aliens[j])) {
          for (int k = 0; k < gameEntities.size(); ++k) {
            if (gameEntities[k] == aliens[j]) {
              gameEntities.erase(gameEntities.begin() + k);
              delete aliens[j];
              aliens.erase(aliens.begin() + j);
            }

            if (gameEntities[k] == bullets[i]) {
              gameEntities.erase(gameEntities.begin() + k);
              delete bullets[i];
              bullets.erase(bullets.begin() + i);
            }

          }
        }
      }
      if (outsideWindow(bullets[i])) {
        for (int k = 0; k < gameEntities.size(); ++k) {
          if (gameEntities[k] == bullets[i]) {
            gameEntities.erase(gameEntities.begin() + k);
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            continue;
          }
        }
      }
    }

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
          default:
            break;
        }
      }
    }

    // Handle keystate
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // Movement
    if (state[SDL_SCANCODE_LEFT]) {
      player->velocity = -0.2f;
    } else if (state[SDL_SCANCODE_RIGHT]) {
      player->velocity = 0.2f;
    } else {
      player->velocity = 0;
    }

    // Shooting
    if (state[SDL_SCANCODE_SPACE]) {
      // Check cooldown on shoot.
      Uint32 time = SDL_GetTicks();
      if (time - lastShot < 500) {
        continue;
      }
      int centerX = player->x + player->dstrect.w / 2;
      Bullet* b = new Bullet(centerX, player->y);
      gameEntities.push_back(b);
      bullets.push_back(b);
      lastShot = time;
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
  if (gameEntity->x + gameEntity->dstrect.w > SCREEN_WIDTH) {
    return true;
  }
  if (gameEntity->y < 0) {
    return true;
  }
  if (gameEntity->y + gameEntity->dstrect.h > SCREEN_HEIGHT) {
    return true;
  }
  return false;
}

bool collision(GameEntity* geA, GameEntity* geB) {
  int topA = geA->dstrect.y;
  int rightA = geA->dstrect.x + geA->dstrect.w;
  int bottomA = geA->dstrect.y + geA->dstrect.h;
  int leftA = geA->dstrect.x;

  int topB = geB->dstrect.y;
  int rightB = geB->dstrect.x + geB->dstrect.w;
  int bottomB = geB->dstrect.y + geB->dstrect.h;
  int leftB = geB->dstrect.x;

  if (bottomA <= topB) {
    return false;
  }
  if (topA >= bottomB) {
    return false;
  }
  if (rightA <= leftB) {
    return false;
  }
  if (leftA >= rightB) {
    return false;
  }
  return true;
}
