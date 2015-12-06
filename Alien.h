#include <SDL2/SDL.h>
#include <iostream>

struct Alien: public GameEntity {
  public:
  Alien(float x, float y, int row);
  ~Alien();
  void update(Uint32 delta);

  private:
  int counter, row_;
  static int direction, shared_y;
  const static int y_step = 3;
};

int Alien::direction = 1;
int Alien::shared_y;

Alien::Alien(float x, float y, int row) {
  this->x = x;
  this->y = y;
  shared_y = y;

  row_ = row;

  velocity = 0.025f;

  bitmap = SDL_LoadBMP("res/invader.bmp");
  if (bitmap == NULL) {
    std::cout << "Unable to load alien bitmap: " << SDL_GetError() << std::endl;
  }

  srcrect = new SDL_Rect();
  srcrect->w = bitmap->w / 2;
  srcrect->h = bitmap->h;

  dstrect.w = srcrect->w * 4;
  dstrect.h = srcrect->h * 2;
}

Alien::~Alien() {
  delete srcrect;
}

void Alien::update(Uint32 delta) {
  // Update animation. Could be improved.
  counter += delta;
  if (counter >= 1000) {
    srcrect->x = (srcrect->x + bitmap->w/2) % bitmap->w;
    counter = 0;
  }

  if (x + dstrect.w > SCREEN_WIDTH - ENEMY_MARGIN) {
    direction = -1;
    shared_y += y_step;
  }

  if (x < ENEMY_MARGIN) {
    direction = 1;
    shared_y += y_step;
  }

  x += delta * velocity * Alien::direction;
  y = shared_y + row_ * 22;
  GameEntity::update(delta);
}
