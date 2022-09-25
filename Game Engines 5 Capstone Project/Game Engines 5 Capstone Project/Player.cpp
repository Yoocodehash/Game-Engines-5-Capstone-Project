#include "Player.h"
#include "TextureManager.h"
#include <iostream>
#include <thread>

Player::Player(const char* texName, int x, int y, int maxHealth_, int currentHealth_)
{
	objectTexture = TextureManager::LoadTexture(texName);

	maxHealth = maxHealth_;
	currentHealth = currentHealth_;
	endGame = false;

	PosX = x;
	PosY = y;
}

Player::~Player()
{
	currentHealth = 0;
	endGame = true;
}

void Player::TakeDamage(int damage_)
{
	currentHealth = maxHealth - damage_;

	std::cout << "You took damage, your health is now at " << currentHealth << "/" << maxHealth << std::endl;
}

int Player::GetHealth()
{
	return currentHealth;
}

bool Player::isGameOver()
{
	return endGame; // returns true to end the game
}

void Player::UpdatePlayer()
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

void Player::RenderPlayer()
{
	SDL_RenderCopy(Window::renderer, objectTexture, &srcRect, &destRect);
}
