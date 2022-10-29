#include "Window.h"
#include "TextureManager.h"
#include "Level.h"
#include <iostream>
#include "Components.h"
#include "Collision.h"
#include "Enemy.h"

PlayerTransformComponent transform;
Controller* gameController = nullptr;

Manager manager;
Level* level;
Enemy* EnemyBird[3];

SDL_Renderer* Window::renderer = nullptr;
SDL_Event Window::event;
SDL_Rect Window::Camera = { 0,0,800, 3500 };

std::vector<ColliderComponent*> Window::colliders;

auto& PlayerBird(manager.AddEntity());
auto& wallBlock(manager.AddEntity());
auto& wallBlock2(manager.AddEntity());

Window::Window(const char* name, int x, int y, int w, int h, bool fullScreen)
{
	int flags = 0;

	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow(name, x, y, w, h, flags);
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Set the color of the renderer
	}

	isRunning = true;

	level = new Level();

	PlayerBird.AddComponent<PlayerTransformComponent>(2);
	PlayerBird.AddComponent<PlayerSpriteComponent>("Flappy bird sprite.png");
	PlayerBird.AddComponent<Controller>();
	PlayerBird.AddComponent<ColliderComponent>("Player");

	wallBlock.AddComponent<PlayerTransformComponent>(1700.0f, 0.0f, 600, 20, 1);
	wallBlock2.AddComponent<PlayerTransformComponent>(-100.0f, 0.0f, 600, 20, 1);
	wallBlock.AddComponent<ColliderComponent>("Wall");
	wallBlock2.AddComponent<ColliderComponent>("Wall2");

	EnemyBird[0] = new Enemy("Enemy Flappy Bird.png", 200, 200);

	isPaused = false;
}

Window::~Window()
{

}

void Window::HandleEvents()
{
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		// The window will either close if you press ESC or press the X button with the mouse
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			// I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
			case SDLK_ESCAPE:
				isRunning = false;
				break;

			case SDLK_p:

				PauseGame = new PauseMenu("Pause Menu", SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, 800, 600, 0);
				
				while (PauseGame->Running())
				{

					PauseGame->UpdatePauseMenu();
					PauseGame->HandleEventPauseMenu();

			case SDL_MOUSEBUTTONUP:
				if (PauseGame->pauseEvent.button.button == SDL_BUTTON_LEFT)
				{
					if (PauseGame->Quit.isSelected)
					{
						PauseGame->threadPool.Finish();
						PauseGame->memoryPool->ReleaseMemoryPool();

						isRunning = false;
						PauseGame->isRunning = false;
						break;
					}
				}

					PauseGame->RenderPauseMenu();
				}

				if (!PauseGame->Running())
				{
					PauseGame->Clear();
				}

				break;
			}

		default:
			break;
		}
	}
}

void Window::Update()
{
	if (!isPaused)
	{
		level->UpdateLevel();
		manager.Refresh();
		manager.Update();

		if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
			wallBlock.GetComponent<ColliderComponent>().collider))
		{
			PlayerBird.GetComponent<PlayerTransformComponent>().velocity * -1;
		}

		if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
			wallBlock2.GetComponent<ColliderComponent>().collider))
		{
			PlayerBird.GetComponent<PlayerTransformComponent>().velocity * -1;
		}

		Camera.x = PlayerBird.GetComponent<PlayerTransformComponent>().position.x - 400;
		Camera.y = PlayerBird.GetComponent<PlayerTransformComponent>().position.y - 320;

		if (Camera.x < 0) Camera.x = 0;
		if (Camera.y < 0) Camera.y = 0;
		if (Camera.x > Camera.w) Camera.x = Camera.w;
		if (Camera.y > Camera.h) Camera.y = Camera.h;

		EnemyBird[0]->UpdateEnemy();
	}

	else 
	{
		PauseGame->UpdatePauseMenu();
	}
}

void Window::Render()
{
	SDL_RenderClear(renderer);
	level->RenderLevel();

	EnemyBird[0]->RenderEnemy();
	manager.Draw();
	SDL_RenderPresent(renderer);

	if (isPaused)
	{
		PauseGame->RenderPauseMenu();
	}
}

void Window::GamePaused()
{
	isPaused = true;
}

void Window::GameUnpaused()
{
	isPaused = false;
	PauseGame = nullptr;
}

void Window::Clear()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Window::Running()
{
	return isRunning;
}
