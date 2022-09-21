#pragma once
#include "Window.h"

class Level
{
public:
	Level(); // This constructor will load the level
	~Level();

	void RenderLevel();

private:
	SDL_Rect srcRect, destRect;
	SDL_Texture* level;
};

