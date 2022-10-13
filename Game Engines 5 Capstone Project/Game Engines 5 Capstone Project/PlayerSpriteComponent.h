#pragma once

#include "Components.h"
#include <SDL.h>
#include "TextureManager.h"

class PlayerSpriteComponent : public Component
{
private:
	PlayerTransformComponent* PlayerPosition;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	int MaxHealth;
	bool EndGame;

public:
	PlayerSpriteComponent() = default;
	PlayerSpriteComponent(const char* fileName_)
	{
		texture = TextureManager::LoadTexture(fileName_);
	}

	~PlayerSpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void SetPlayerHealth(int currentHealth_, int damage_)
	{
		MaxHealth = currentHealth_;
		int lostHealth = MaxHealth - damage_;

		std::cout << "You took damage, your health is now at " << lostHealth << "/" << MaxHealth << std::endl;
	}

	bool IsGameOver()
	{
		return EndGame; // returns true to end the game
	}

	void PlayerInit() override
	{
		PlayerPosition = &entity->GetComponent<PlayerTransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.h = PlayerPosition->height;
		srcRect.w = PlayerPosition->width;
	}

	void UpdatePlayer() override
	{
		destRect.x = static_cast<int>(PlayerPosition->position.x) - Window::Camera.x;
		destRect.y = static_cast<int>(PlayerPosition->position.y) - Window::Camera.y;
		destRect.w = PlayerPosition->width * PlayerPosition->scale;
		destRect.h = PlayerPosition->height * PlayerPosition->scale;
	}

	void Draw() override
	{
		TextureManager::DrawTexture(texture, srcRect, destRect);
	}
};