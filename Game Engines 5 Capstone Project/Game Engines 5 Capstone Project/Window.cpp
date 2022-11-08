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

	anyAudio.LoadAudio();

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

				//Pause the music and any sound effects playing
				Mix_PauseMusic();
				Mix_HaltChannel(-1);

				PauseGame = new PauseMenu("Pause Menu", SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, 800, 600, 0);

				Mix_PlayChannel(-1, anyAudio.PauseSound, 0);
				
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

					//If the music is paused
					if (Mix_PausedMusic() == 1)
					{
						//Resume the music
						Mix_ResumeMusic();
					}
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


		Camera.x = PlayerBird.GetComponent<PlayerTransformComponent>().position.x - 400.0f;
		Camera.y = PlayerBird.GetComponent<PlayerTransformComponent>().position.y - 320.0f;

		if (Camera.x < 0) Camera.x = 0;
		if (Camera.y < 0) Camera.y = 0;
		if (Camera.x > Camera.w) Camera.x = Camera.w;
		if (Camera.y > 5) Camera.y = 5;

		EnemyBird[0]->UpdateEnemy();
	}

	else 
	{
		PauseGame->UpdatePauseMenu();
	}
}

void Window::InFrustum()
{
	// These numbers are hard-coded to give better results when the player is outside of the camera view

	if (PlayerBird.GetComponent<PlayerTransformComponent>().position.x > -150 // Within left side of frustum
		&& PlayerBird.GetComponent<PlayerTransformComponent>().position.x < 1545 // Within right side of frustum
		&& PlayerBird.GetComponent<PlayerTransformComponent>().position.y > -100 // Below top side of frustum
		&& PlayerBird.GetComponent<PlayerTransformComponent>().position.y < 570) // Above bottom of frustum
	{
		std::cout << "The player is inside the camera frustum\n";
	}

	else // If the player is outside of the frustum
	{
		std::cout << "The player is outside the camera frustum\n";
	}
}

void Window::ShowLevelCompleteScreen()
{
	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		wallBlock.GetComponent<ColliderComponent>().collider))
	{
		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		LevelFinished = new LevelCompleteScreen("Level Completed Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

		while (LevelFinished->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			LevelFinished->UpdateLevelCompleteScreen();
			LevelFinished->HandleEventLevelCompleteScreen();

			if (LevelFinished->LevelCompleteEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFinished->Exit.isSelected)
				{
					LevelFinished->threadPool.Finish();
					LevelFinished->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFinished->isRunning = false;
					break;
				}
			}

			LevelFinished->RenderLevelCompleteScreen();

			isRunning = false;
			LevelFinished->isRunning = true;
		}

		if (!LevelFinished->Running())
		{
			LevelFinished->Clear();
		}
	}
}

void Window::ShowLevelFailedScreen()
{
	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		wallBlock2.GetComponent<ColliderComponent>().collider))
	{
		LevelFailed = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed->UpdateGameOver();
			LevelFailed->HandleEventGameOver();

			if (LevelFailed->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed->Quit.isSelected)
				{
					LevelFailed->threadPool.Finish();
					LevelFailed->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed->isRunning = false;
					break;
				}
			}

			LevelFailed->RenderGameOver();

			isRunning = false;
			LevelFailed->isRunning = true;
		}

		if (!LevelFailed->Running())
		{
			LevelFailed->Clear();
		}
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
