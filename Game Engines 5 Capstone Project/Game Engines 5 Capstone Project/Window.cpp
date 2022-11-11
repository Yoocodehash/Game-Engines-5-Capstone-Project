#include "Window.h"
#include "TextureManager.h"
#include "Level.h"
#include <iostream>
#include "Components.h"
#include "Collision.h"

PlayerTransformComponent transform;
Controller* gameController = nullptr;

Manager manager;
Level* level;

SDL_Renderer* Window::renderer = nullptr;
SDL_Event Window::event;
SDL_Rect Window::Camera = { 0,0,800, 3500 };

auto& PlayerBird(manager.AddEntity());
auto& wallBlock(manager.AddEntity());
auto& wallBlock2(manager.AddEntity());
auto& spike1(manager.AddEntity());
auto& spike2(manager.AddEntity());
auto& spike3(manager.AddEntity());
auto& spike4(manager.AddEntity());
auto& spike5(manager.AddEntity());
auto& spike6(manager.AddEntity());
auto& spike7(manager.AddEntity());
auto& spike8(manager.AddEntity());
auto& spike9(manager.AddEntity());
auto& spike10(manager.AddEntity());
auto& spike11(manager.AddEntity());
auto& spike12(manager.AddEntity());
auto& spike13(manager.AddEntity());
auto& spike14(manager.AddEntity());
auto& spike15(manager.AddEntity());
auto& spike16(manager.AddEntity());
auto& spike17(manager.AddEntity());
auto& spike18(manager.AddEntity());

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

	spike1.AddComponent<PlayerTransformComponent>(20.0f, 0.0f, 150, 100, 1);
	spike2.AddComponent<PlayerTransformComponent>(20.0f, 450.0f, 150, 100, 1);
	spike3.AddComponent<PlayerTransformComponent>(220.0f, 0.0f, 150, 100, 1);
	spike4.AddComponent<PlayerTransformComponent>(220.0f, 450.0f, 150, 100, 1);
	spike5.AddComponent<PlayerTransformComponent>(420.0f, 0.0f, 150, 100, 1);
	spike6.AddComponent<PlayerTransformComponent>(420.0f, 450.0f, 150, 100, 1);
	spike7.AddComponent<PlayerTransformComponent>(620.0f, 0.0f, 150, 100, 1);
	spike8.AddComponent<PlayerTransformComponent>(620.0f, 450.0f, 150, 100, 1);
	spike9.AddComponent<PlayerTransformComponent>(820.0f, 0.0f, 150, 100, 1);
	spike10.AddComponent<PlayerTransformComponent>(820.0f, 450.0f, 150, 100, 1);
	spike11.AddComponent<PlayerTransformComponent>(1020.0f, 0.0f, 150, 100, 1);
	spike12.AddComponent<PlayerTransformComponent>(1020.0f, 450.0f, 150, 100, 1);
	spike13.AddComponent<PlayerTransformComponent>(1220.0f, 0.0f, 150, 100, 1);
	spike14.AddComponent<PlayerTransformComponent>(1220.0f, 450.0f, 150, 100, 1);
	spike15.AddComponent<PlayerTransformComponent>(1420.0f, 0.0f, 150, 100, 1);
	spike16.AddComponent<PlayerTransformComponent>(1420.0f, 450.0f, 150, 100, 1);
	spike17.AddComponent<PlayerTransformComponent>(1620.0f, 0.0f, 150, 100, 1);
	spike18.AddComponent<PlayerTransformComponent>(1620.0f, 450.0f, 150, 100, 1);
	spike1.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike2.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike3.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike4.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike5.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike6.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike7.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike8.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike9.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike10.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike11.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike12.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike13.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike14.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike15.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike16.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike17.AddComponent<PlayerSpriteComponent>("Long Metal Spike Rotated Vertically.png");
	spike18.AddComponent<PlayerSpriteComponent>("Long Metal Spike.png");
	spike1.AddComponent<ColliderComponent>("Spike1");
	spike2.AddComponent<ColliderComponent>("Spike2");
	spike3.AddComponent<ColliderComponent>("Spike3");
	spike4.AddComponent<ColliderComponent>("Spike4");
	spike5.AddComponent<ColliderComponent>("Spike5");
	spike6.AddComponent<ColliderComponent>("Spike6");
	spike7.AddComponent<ColliderComponent>("Spike7");
	spike8.AddComponent<ColliderComponent>("Spike8");
	spike9.AddComponent<ColliderComponent>("Spike9");
	spike10.AddComponent<ColliderComponent>("Spike10");
	spike11.AddComponent<ColliderComponent>("Spike11");
	spike12.AddComponent<ColliderComponent>("Spike12");
	spike13.AddComponent<ColliderComponent>("Spike13");
	spike14.AddComponent<ColliderComponent>("Spike14");
	spike15.AddComponent<ColliderComponent>("Spike15");
	spike16.AddComponent<ColliderComponent>("Spike16");
	spike17.AddComponent<ColliderComponent>("Spike17");
	spike18.AddComponent<ColliderComponent>("Spike18");

	PlayerBird.AddComponent<PlayerTransformComponent>(-50.0f, 250.0f, 69, 100, 2);
	PlayerBird.AddComponent<PlayerSpriteComponent>("Flappy bird sprite.png");
	PlayerBird.AddComponent<Controller>();
	PlayerBird.AddComponent<ColliderComponent>("Player");

	wallBlock.AddComponent<PlayerTransformComponent>(1700.0f, 0.0f, 600, 20, 1);
	wallBlock2.AddComponent<PlayerTransformComponent>(-100.0f, 0.0f, 600, 20, 1);
	wallBlock.AddComponent<ColliderComponent>("Wall");
	wallBlock2.AddComponent<ColliderComponent>("Wall2");

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
		GameOverScreen* LevelFailed = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
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

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike1.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed2 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed2->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed2->UpdateGameOver();
			LevelFailed2->HandleEventGameOver();

			if (LevelFailed2->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed2->Quit.isSelected)
				{
					LevelFailed2->threadPool.Finish();
					LevelFailed2->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed2->isRunning = false;
					break;
				}
			}

			LevelFailed2->RenderGameOver();

			isRunning = false;
			LevelFailed2->isRunning = true;
		}

		if (!LevelFailed2->Running())
		{
			LevelFailed2->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike2.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed3 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed3->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed3->UpdateGameOver();
			LevelFailed3->HandleEventGameOver();

			if (LevelFailed3->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed3->Quit.isSelected)
				{
					LevelFailed3->threadPool.Finish();
					LevelFailed3->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed3->isRunning = false;
					break;
				}
			}

			LevelFailed3->RenderGameOver();

			isRunning = false;
			LevelFailed3->isRunning = true;
		}

		if (!LevelFailed3->Running())
		{
			LevelFailed3->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike3.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed4 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed4->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed4->UpdateGameOver();
			LevelFailed4->HandleEventGameOver();

			if (LevelFailed4->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed4->Quit.isSelected)
				{
					LevelFailed4->threadPool.Finish();
					LevelFailed4->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed4->isRunning = false;
					break;
				}
			}

			LevelFailed4->RenderGameOver();

			isRunning = false;
			LevelFailed4->isRunning = true;
		}

		if (!LevelFailed4->Running())
		{
			LevelFailed4->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike4.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed5 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed5->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed5->UpdateGameOver();
			LevelFailed5->HandleEventGameOver();

			if (LevelFailed5->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed5->Quit.isSelected)
				{
					LevelFailed5->threadPool.Finish();
					LevelFailed5->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed5->isRunning = false;
					break;
				}
			}

			LevelFailed5->RenderGameOver();

			isRunning = false;
			LevelFailed5->isRunning = true;
		}

		if (!LevelFailed5->Running())
		{
			LevelFailed5->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike5.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed6 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed6->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed6->UpdateGameOver();
			LevelFailed6->HandleEventGameOver();

			if (LevelFailed6->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed6->Quit.isSelected)
				{
					LevelFailed6->threadPool.Finish();
					LevelFailed6->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed6->isRunning = false;
					break;
				}
			}

			LevelFailed6->RenderGameOver();

			isRunning = false;
			LevelFailed6->isRunning = true;
		}

		if (!LevelFailed6->Running())
		{
			LevelFailed6->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike6.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed7 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed7->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed7->UpdateGameOver();
			LevelFailed7->HandleEventGameOver();

			if (LevelFailed7->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed7->Quit.isSelected)
				{
					LevelFailed7->threadPool.Finish();
					LevelFailed7->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed7->isRunning = false;
					break;
				}
			}

			LevelFailed7->RenderGameOver();

			isRunning = false;
			LevelFailed7->isRunning = true;
		}

		if (!LevelFailed7->Running())
		{
			LevelFailed7->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike7.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed8 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed8->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed8->UpdateGameOver();
			LevelFailed8->HandleEventGameOver();

			if (LevelFailed8->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed8->Quit.isSelected)
				{
					LevelFailed8->threadPool.Finish();
					LevelFailed8->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed8->isRunning = false;
					break;
				}
			}

			LevelFailed8->RenderGameOver();

			isRunning = false;
			LevelFailed8->isRunning = true;
		}

		if (!LevelFailed8->Running())
		{
			LevelFailed8->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike8.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed9 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed9->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed9->UpdateGameOver();
			LevelFailed9->HandleEventGameOver();

			if (LevelFailed9->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed9->Quit.isSelected)
				{
					LevelFailed9->threadPool.Finish();
					LevelFailed9->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed9->isRunning = false;
					break;
				}
			}

			LevelFailed9->RenderGameOver();

			isRunning = false;
			LevelFailed9->isRunning = true;
		}

		if (!LevelFailed9->Running())
		{
			LevelFailed9->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike9.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed10 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed10->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed10->UpdateGameOver();
			LevelFailed10->HandleEventGameOver();

			if (LevelFailed10->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed10->Quit.isSelected)
				{
					LevelFailed10->threadPool.Finish();
					LevelFailed10->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed10->isRunning = false;
					break;
				}
			}

			LevelFailed10->RenderGameOver();

			isRunning = false;
			LevelFailed10->isRunning = true;
		}

		if (!LevelFailed10->Running())
		{
			LevelFailed10->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike10.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed11 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed11->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed11->UpdateGameOver();
			LevelFailed11->HandleEventGameOver();

			if (LevelFailed11->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed11->Quit.isSelected)
				{
					LevelFailed11->threadPool.Finish();
					LevelFailed11->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed11->isRunning = false;
					break;
				}
			}

			LevelFailed11->RenderGameOver();

			isRunning = false;
			LevelFailed11->isRunning = true;
		}

		if (!LevelFailed11->Running())
		{
			LevelFailed11->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike11.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed12 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed12->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed12->UpdateGameOver();
			LevelFailed12->HandleEventGameOver();

			if (LevelFailed12->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed12->Quit.isSelected)
				{
					LevelFailed12->threadPool.Finish();
					LevelFailed12->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed12->isRunning = false;
					break;
				}
			}

			LevelFailed12->RenderGameOver();

			isRunning = false;
			LevelFailed12->isRunning = true;
		}

		if (!LevelFailed12->Running())
		{
			LevelFailed12->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike12.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed13 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed13->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed13->UpdateGameOver();
			LevelFailed13->HandleEventGameOver();

			if (LevelFailed13->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed13->Quit.isSelected)
				{
					LevelFailed13->threadPool.Finish();
					LevelFailed13->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed13->isRunning = false;
					break;
				}
			}

			LevelFailed13->RenderGameOver();

			isRunning = false;
			LevelFailed13->isRunning = true;
		}

		if (!LevelFailed13->Running())
		{
			LevelFailed13->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike13.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed14 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed14->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed14->UpdateGameOver();
			LevelFailed14->HandleEventGameOver();

			if (LevelFailed14->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed14->Quit.isSelected)
				{
					LevelFailed14->threadPool.Finish();
					LevelFailed14->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed14->isRunning = false;
					break;
				}
			}

			LevelFailed14->RenderGameOver();

			isRunning = false;
			LevelFailed14->isRunning = true;
		}

		if (!LevelFailed14->Running())
		{
			LevelFailed14->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike14.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed15 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed15->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed15->UpdateGameOver();
			LevelFailed15->HandleEventGameOver();

			if (LevelFailed15->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed15->Quit.isSelected)
				{
					LevelFailed15->threadPool.Finish();
					LevelFailed15->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed15->isRunning = false;
					break;
				}
			}

			LevelFailed15->RenderGameOver();

			isRunning = false;
			LevelFailed15->isRunning = true;
		}

		if (!LevelFailed15->Running())
		{
			LevelFailed15->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike15.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed16 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed16->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed16->UpdateGameOver();
			LevelFailed16->HandleEventGameOver();

			if (LevelFailed16->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed16->Quit.isSelected)
				{
					LevelFailed16->threadPool.Finish();
					LevelFailed16->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed16->isRunning = false;
					break;
				}
			}

			LevelFailed16->RenderGameOver();

			isRunning = false;
			LevelFailed16->isRunning = true;
		}

		if (!LevelFailed16->Running())
		{
			LevelFailed16->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike16.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed17 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed17->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed17->UpdateGameOver();
			LevelFailed17->HandleEventGameOver();

			if (LevelFailed17->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed17->Quit.isSelected)
				{
					LevelFailed17->threadPool.Finish();
					LevelFailed17->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed17->isRunning = false;
					break;
				}
			}

			LevelFailed17->RenderGameOver();

			isRunning = false;
			LevelFailed17->isRunning = true;
		}

		if (!LevelFailed17->Running())
		{
			LevelFailed17->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike17.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed18 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed18->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed18->UpdateGameOver();
			LevelFailed18->HandleEventGameOver();

			if (LevelFailed18->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed18->Quit.isSelected)
				{
					LevelFailed18->threadPool.Finish();
					LevelFailed18->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed18->isRunning = false;
					break;
				}
			}

			LevelFailed18->RenderGameOver();

			isRunning = false;
			LevelFailed18->isRunning = true;
		}

		if (!LevelFailed18->Running())
		{
			LevelFailed18->Clear();
		}
	}

	if (Collision::AABB(PlayerBird.GetComponent<ColliderComponent>().collider,
		spike18.GetComponent<ColliderComponent>().collider))
	{
		GameOverScreen* LevelFailed19 = new GameOverScreen("Game Over Screen", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 600, 0);

		//Stop the music and sound playing in the game
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		Mix_PlayChannel(-1, anyAudio.GameOverSound, 0);

		while (LevelFailed19->Running())
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			//Mix_PlayChannel(-1, anyAudio.LevelCompletionSound, 0);

			LevelFailed19->UpdateGameOver();
			LevelFailed19->HandleEventGameOver();

			if (LevelFailed19->gameoverEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (LevelFailed19->Quit.isSelected)
				{
					LevelFailed19->threadPool.Finish();
					LevelFailed19->memoryPool->ReleaseMemoryPool();

					isRunning = false;
					LevelFailed19->isRunning = false;
					break;
				}
			}

			LevelFailed19->RenderGameOver();

			isRunning = false;
			LevelFailed19->isRunning = true;
		}

		if (!LevelFailed19->Running())
		{
			LevelFailed19->Clear();
		}
	}
}

void Window::Render()
{
	SDL_RenderClear(renderer);
	level->RenderLevel();

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
