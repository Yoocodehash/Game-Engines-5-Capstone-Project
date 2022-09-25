#pragma once

#include "Window.h"

class Enemy
{
public:
	Enemy(const char* texName, int x, int y);
	~Enemy();

	void UpdateEnemy();
	void RenderEnemy();

private:
	int PosX;
	int PosY;

	SDL_Texture* objectTexture;
	SDL_Rect srcRect, destRect;
};