#include <SDL2/SDL.h>
#include <iostream>

struct Bullet: public GameEntity {
  Bullet(float x, float y);
  void update(Uint32 delta);
};

Bullet::Bullet(float x, float y) {
  this->x = x;
  this->y = y;

  velocity = 0.8f;

  bitmap = SDL_LoadBMP("res/bullet.bmp");
  if (bitmap == NULL) {
    std::cout << "Unable to load bullet bitmap: " << SDL_GetError() << std::endl;
  }

  dstrect.w = bitmap->w;
  dstrect.h = bitmap->h;
}

void Bullet::update(Uint32 delta) {
  y -= delta * velocity;
  GameEntity::update(delta);
}
