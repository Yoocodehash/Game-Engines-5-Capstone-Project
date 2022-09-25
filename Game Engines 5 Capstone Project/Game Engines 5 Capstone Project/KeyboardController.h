#pragma once

#include "Window.h"
#include "EntityComponentSystem.h"
#include "Components.h"

class KeyboardController : public Component
{
public: 
	PlayerTransformComponent* PlayerTransform;

	void PlayerInit() override
	{
		PlayerTransform = &entity->GetComponent<PlayerTransformComponent>();
	}

	void UpdatePlayer() override
	{
		if (Window::event.type == SDL_KEYDOWN)
		{
			switch (Window::event.key.keysym.sym)
			{
			case SDLK_s:
				PlayerTransform->velocity.y = 0.5;
				break;
			case SDLK_a:
				PlayerTransform->velocity.x = -0.5;
				break;
			case SDLK_w:
				PlayerTransform->velocity.y = -0.5;
				break;
			case SDLK_d:
				PlayerTransform->velocity.x = 0.5;
				break;
			default:
				break;
			}
		}

		if (Window::event.type == SDL_KEYUP)
		{
			switch (Window::event.key.keysym.sym)
			{
			case SDLK_s:
				PlayerTransform->velocity.y = 0;
				break;
			case SDLK_a:
				PlayerTransform->velocity.x = 0;
				break;
			case SDLK_w:
				PlayerTransform->velocity.y = 0;
				break;
			case SDLK_d:
				PlayerTransform->velocity.x = 0;
				break;
			default:
				break;
			}
		}
	}
};