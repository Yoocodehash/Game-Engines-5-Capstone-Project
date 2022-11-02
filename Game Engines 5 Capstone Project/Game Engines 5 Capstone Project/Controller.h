#pragma once

#include "Window.h"
#include "EntityComponentSystem.h"
#include "Components.h"
#include "Audio.h"

#include <Windows.h>
#include <Xinput.h>

class Controller : public Component
{
private:
	SDL_Joystick* player;
	Audio soundEffect;

public:
	PlayerTransformComponent* PlayerTransform;

	void PlayerInit() override
	{
		PlayerTransform = &entity->GetComponent<PlayerTransformComponent>();
		soundEffect.LoadAudio();

		SDL_Init(SDL_INIT_JOYSTICK);

		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks connected!\n\n");
		}
		else
		{
			//Load joystick
			player = SDL_JoystickOpen(0);
			if (player == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}
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
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;
			case SDLK_LEFT:
				PlayerTransform->velocity.x = -0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;
			case SDLK_UP:
				PlayerTransform->velocity.y = -0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;
			case SDLK_RIGHT:
				PlayerTransform->velocity.x = 0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
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

		if (Window::event.type == SDL_JOYBUTTONDOWN)
		{
			switch (Window::event.jbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A:
				PlayerTransform->velocity.y = 0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;
			case SDL_CONTROLLER_BUTTON_B:
				PlayerTransform->velocity.x = 0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;
			case SDL_CONTROLLER_BUTTON_X:
				PlayerTransform->velocity.x = -0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				PlayerTransform->velocity.y = -0.5;
				Mix_PlayChannel(-1, soundEffect.BirdSound, -1);
				break;

			default:
				break;
			}
		}

		if (Window::event.type == SDL_JOYBUTTONUP)
		{
			switch (Window::event.jbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A:
				PlayerTransform->velocity.y = 0;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				PlayerTransform->velocity.x = 0;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				PlayerTransform->velocity.x = 0;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				PlayerTransform->velocity.y = 0;
				break;

			default:
				break;
			}
		}
	}
};