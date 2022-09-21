#include "Level.h"
#include "TextureManager.h"

Level::Level()
{
	destRect.w = 3674;
	destRect.h = 550;
	destRect.x = 0;
	destRect.y = 0;

	srcRect.w = destRect.w / 2;
	srcRect.h = destRect.h / 2;
	srcRect.x = 0;
	srcRect.y = 0;

	level = TextureManager::LoadTexture("Level 1.png");
}

Level::~Level()
{
}

void Level::RenderLevel()
{
	SDL_RenderCopy(Window::renderer, level, &srcRect, &destRect);
}
