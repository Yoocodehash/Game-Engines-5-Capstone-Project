#pragma once

#include "Window.h"
#include "EntityComponentSystem.h"
#include "Components.h"

//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include <Xinput.h>

class Controller : public Component
{
private:
	//int controllerID;
	//XINPUT_STATE controllerState{};

	//Analog joystick dead zone
	const int JOYSTICK_DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_Joystick* gGameController = NULL;

	//Normalized direction
	int xDir = 0;
	int yDir = 0;

	//Main loop flag
	bool quit = false;

public: 
	/*Controller* player0;

	Controller(int playerID) : controllerID(playerID)
	{
		memset(&controllerState, 0, sizeof(controllerState));
	}

	XINPUT_STATE GetState() // has unresolved error here
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

		std::cerr << "No controller " << controllerID << "found\n";
		return false;
	} */

	PlayerTransformComponent* PlayerTransform;

	void PlayerInit() override
	{
		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks connected!\n");
		}
		else
		{
			//Load joystick
			gGameController = SDL_JoystickOpen(0);
			if (gGameController == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}

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

		if (Window::event.type == SDL_JOYAXISMOTION)
		{
			//Motion on controller 0
			if (Window::event.jaxis.which == 0)
			{
				//X axis motion
				if (Window::event.jaxis.axis == 0)
				{
					//Left of dead zone
					if (Window::event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						PlayerTransform->velocity.x = -1;
					}
					//Right of dead zone
					else if (Window::event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						PlayerTransform->velocity.x = 1;
					}
					else
					{
						PlayerTransform->velocity.x = 0;
					}
				}

				//Y axis motion
				else if (Window::event.jaxis.axis == 1)
				{
					//Below of dead zone
					if (Window::event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						PlayerTransform->velocity.y = -1;
					}
					//Above of dead zone
					else if (Window::event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						PlayerTransform->velocity.y = 1;
					}
					else
					{
						PlayerTransform->velocity.y = 0;
					}
				}

				//Calculate angle
				double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);

				//Correct angle
				if (xDir == 0 && yDir == 0)
				{
					joystickAngle = 0;
				}

			}
		}

		/*player0 = new Controller(0);

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

		delete player0; */
	}
};