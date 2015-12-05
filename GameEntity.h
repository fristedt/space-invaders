#include <SDL2/SDL.h>

struct GameEntity {
  float x, y, velocity;
  SDL_Surface* bitmap;
  SDL_Rect *srcrect, dstrect;

  GameEntity() {
    srcrect = nullptr;
  }

  ~GameEntity() {
    SDL_FreeSurface(bitmap);
  };
  virtual void update(Uint32 delta);
};

void GameEntity::update(Uint32 delta) {
  dstrect.x = (int)x;
  dstrect.y = (int)y;
}
