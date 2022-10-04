#pragma once

#include "Window.h"
#include "EntityComponentSystem.h"
#include "Components.h"
#include <Windows.h>
#include <Xinput.h>

class Controller : public Component
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
				// Keyboard controls
			case SDLK_DOWN:
				PlayerTransform->velocity.y = 0.5;
				break;
			case SDLK_LEFT:
				PlayerTransform->velocity.x = -0.5;
				break;
			case SDLK_UP:
				PlayerTransform->velocity.y = -0.5;
				break;
			case SDLK_RIGHT:
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
			case SDLK_DOWN:
				PlayerTransform->velocity.y = 0;
				break;
			case SDLK_LEFT:
				PlayerTransform->velocity.x = 0;
				break;
			case SDLK_UP:
				PlayerTransform->velocity.y = 0;
				break;
			case SDLK_RIGHT:
				PlayerTransform->velocity.x = 0;
				break;
			default:
				break;
			}
		}

		if (Window::event.type == SDL_JOYBUTTONDOWN) // When a Xbox button is pressed
		{
			switch (Window::event.key.keysym.sym)
			{
				// XBOX controls
			case XINPUT_GAMEPAD_A:
				PlayerTransform->velocity.y = 0.5;
				break;
			case XINPUT_GAMEPAD_B:
				PlayerTransform->velocity.x = -0.5;
				break;
			case XINPUT_GAMEPAD_X:
				PlayerTransform->velocity.y = -0.5;
				break;
			case XINPUT_GAMEPAD_Y:
				PlayerTransform->velocity.x = 0.5;
				break;
			}
		}

		if (Window::event.type == SDL_JOYBUTTONUP) // When a Xbox button is released
		{
			switch (Window::event.key.keysym.sym)
			{
				// XBOX controls
			case XINPUT_GAMEPAD_A:
				PlayerTransform->velocity.y = 0;
				break;
			case XINPUT_GAMEPAD_B:
				PlayerTransform->velocity.x = 0;
				break;
			case XINPUT_GAMEPAD_X:
				PlayerTransform->velocity.y = 0;
				break;
			case XINPUT_GAMEPAD_Y:
				PlayerTransform->velocity.x = 0;
				break;
			}
		}
	}
};