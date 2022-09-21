#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texName, int x, int y)
{
	objectTexture = TextureManager::LoadTexture(texName);

	PosX = x;
	PosY = y;
}

GameObject::~GameObject()
{
}

void GameObject::UpdatePlayer()
{
	//PosX++;
	//PosY--; 
	
	srcRect.h = 69;
	srcRect.w = 100; 
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = PosX;
	destRect.y = PosY;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
}

void GameObject::Render()
{
	SDL_RenderCopy(Window::renderer, objectTexture, &srcRect, &destRect);
}
