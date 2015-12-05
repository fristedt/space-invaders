all: space_invaders
space_invaders: game.cpp constants.h Player.h Alien.h Bullet.h GameEntity.h
	g++ -std=c++14 game.cpp -o space_invaders -lSDL2
