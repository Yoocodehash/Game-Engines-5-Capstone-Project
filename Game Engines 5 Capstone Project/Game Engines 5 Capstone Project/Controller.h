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
	int controllerID;
	XINPUT_STATE controllerState{};

	Audio soundEffect;

public:
	Controller* player0;
	PlayerTransformComponent* PlayerTransform;

	void PlayerInit() override
	{
		PlayerTransform = &entity->GetComponent<PlayerTransformComponent>();
		soundEffect.LoadAudio();
	}

	void XboxController(int playerID)
	{
		controllerID = playerID;
		memset(&controllerState, 0, sizeof(controllerState));
	}

	XINPUT_STATE GetState()
	{
		XInputGetState(controllerID, &controllerState);
		return controllerState;
	}

	bool IsConnected()
	{
		uint16_t result = XInputGetState(controllerID, &controllerState);

		if (result == ERROR_SUCCESS)
		{
			return true;
		}

		std::cerr << "No controller " << controllerID << " found!\n";
		return false;
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

		player0 = new Controller();
		player0->XboxController(0);

		while (player0->IsConnected())
		{
			XINPUT_STATE xinput_state = player0->GetState();

			if (xinput_state.Gamepad.wButtons > 0) // When a Xbox button is pressed
			{
				switch (xinput_state.Gamepad.wButtons)
				{
					// XBOX controls
				case XINPUT_GAMEPAD_A:
					player0->PlayerTransform->velocity.y = 0.5;
					break;
				case XINPUT_GAMEPAD_B:
					player0->PlayerTransform->velocity.x = -0.5;
					break;
				case XINPUT_GAMEPAD_X:
					player0->PlayerTransform->velocity.y = -0.5;
					break;
				case XINPUT_GAMEPAD_Y:
					player0->PlayerTransform->velocity.x = 0.5;
					break;

				default:
					static int count = 0;
					count++;
					break;
				}
			}

			if (xinput_state.Gamepad.wButtons < 0) // When a Xbox button is released
			{
				switch (xinput_state.Gamepad.wButtons)
				{
					// XBOX controls
				case XINPUT_GAMEPAD_A:
					player0->PlayerTransform->velocity.y = 0;
					break;
				case XINPUT_GAMEPAD_B:
					player0->PlayerTransform->velocity.x = 0;
					break;
				case XINPUT_GAMEPAD_X:
					player0->PlayerTransform->velocity.y = 0;
					break;
				case XINPUT_GAMEPAD_Y:
					player0->PlayerTransform->velocity.x = 0;
					break;

				default:
					static int count = 0;
					count++;
					break;
				}
			}
		}

		delete player0;
	}
};