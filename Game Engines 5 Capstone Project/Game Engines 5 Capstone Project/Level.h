#pragma once
#include "Window.h"
#include "Vec2.h"

class Level
{
public:
	Vec2 position;

	Level(); // This constructor will load the level
	~Level();

	void UpdateLevel();
	void RenderLevel();

private:
	SDL_Rect srcRect, destRect;
	SDL_Texture* level;
};

