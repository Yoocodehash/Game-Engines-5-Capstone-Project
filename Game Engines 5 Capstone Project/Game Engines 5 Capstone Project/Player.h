#pragma once

#include "Window.h"

class Player
{
public:
	Player(const char* texName, int x, int y, int maxHealth_, int currentHealth_);
	~Player(); // This deconstructor will take place if player has no health

	void TakeDamage(int damage_);
	int GetHealth();
	bool isGameOver();

	void UpdatePlayer();
	void RenderPlayer();

private:
	int PosX; 
	int PosY;

	int maxHealth;
	int currentHealth;
	bool endGame;

	SDL_Texture* objectTexture;
	SDL_Rect srcRect, destRect;
};