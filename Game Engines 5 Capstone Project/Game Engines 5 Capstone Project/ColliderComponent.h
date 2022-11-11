#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	PlayerTransformComponent* transform;

	ColliderComponent(std::string tag_)
	{
		tag = tag_;
	}

	void PlayerInit() override
	{
		if (!entity->HasComponent<PlayerTransformComponent>())
		{
			entity->AddComponent<PlayerTransformComponent>();
		}

		transform = &entity->GetComponent<PlayerTransformComponent>();
	}

	void UpdatePlayer() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};
