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
	void SpriteComponent(const char* fileName_)
	{
		texture = TextureManager::LoadTexture(fileName_);
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
		srcRect.h = 69;
		srcRect.w = 100;

		destRect.w = srcRect.w * 2;
		destRect.h = srcRect.h * 2;
	}

	void UpdatePlayer() override
	{
		destRect.x = PlayerPosition->position.x;
		destRect.y = PlayerPosition->position.y;
	}

	void Draw() override
	{
		TextureManager::DrawTexture(texture, srcRect, destRect);
	}
};