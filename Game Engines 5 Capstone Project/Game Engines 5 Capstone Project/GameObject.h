#pragma once

#include "Window.h"

class GameObject
{
public:
	GameObject(const char* texName, int x, int y);
	~GameObject();

	void UpdatePlayer();
	void Render();

private:
	int PosX; 
	int PosY;

	SDL_Texture* objectTexture;
	SDL_Rect srcRect, destRect;
};

