all: space_invaders
space_invaders: game.cpp
	g++ -std=c++14 game.cpp -o space_invaders -lSDL2
