#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy(const char* texName, int x, int y)
{
	objectTexture = TextureManager::LoadTexture(texName);

	PosX = x;
	PosY = y;
}

Enemy::~Enemy()
{
}

void Enemy::UpdateEnemy()
{
	//PosX++;
	//PosY--; 

	srcRect.h = 77;
	srcRect.w = 100;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = PosX;
	destRect.y = PosY;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
}

void Enemy::RenderEnemy()
{
	SDL_RenderCopy(Window::renderer, objectTexture, &srcRect, &destRect);
}