#include "Window.h"
#include "TextureManager.h"
#include "Level.h"
#include <iostream>
#include "Components.h"
#include "Enemy.h"
#include <Windows.h>
#include <Xinput.h>

PlayerTransformComponent transform;

Manager manager;
Level* level;
Enemy* EnemyBird[3];

SDL_Renderer* Window::renderer = nullptr;
SDL_Event Window::event;
SDL_Rect Window::Camera = { 0,0,800, 3500 };

auto& PlayerBird(manager.AddEntity());

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

	PlayerBird.AddComponent<PlayerTransformComponent>();
	PlayerBird.AddComponent<PlayerSpriteComponent>().SpriteComponent("Flappy bird sprite.png");
	PlayerBird.AddComponent<Controller>();
	PlayerBird.AddComponent<PlayerSpriteComponent>().SetPlayerHealth(10, 3);

	EnemyBird[0] = new Enemy("Enemy Flappy Bird.png", 200, 200);

	level = new Level();
}

Window::~Window()
{

}

void Window::RunMultithread()
{
	// (function, object pointer, starting point, end point, milliseconds (used for std::chrono))
	std::thread thread1(&Window::PlayerAt, ourWindow, 0, 10, 500);
	thread1.detach();

	// (function, object pointer, starting point, end point, milliseconds (used for std::chrono))
	std::thread thread2(&Window::Level1, ourWindow, 0, 10, 500);
	thread2.detach();

	// (function, object pointer, starting point, end point, milliseconds (used for std::chrono))
	std::thread thread3(&Window::RunGame, ourWindow, 0, 10, 500);
	thread3.detach();
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

		case SDL_JOYBUTTONDOWN:
			switch (event.key.keysym.sym)
			{
				// Xbox controls to exit the window
			case XINPUT_GAMEPAD_BACK:
				isRunning = false;
				break;
			}

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			// I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			}

		default:
			break;
		}
	}
}

void Window::Update()
{
	manager.Refresh();
	manager.Update();

	/*Camera.x = PlayerBird.GetComponent<PlayerTransformComponent>().position.x - 400;
	Camera.y = PlayerBird.GetComponent<PlayerTransformComponent>().position.y - 320;

	if (Camera.x < 0) Camera.x = 0;
	if (Camera.y < 0) Camera.y = 0;
	if (Camera.x > Camera.w) Camera.x = Camera.w;
	if (Camera.y > Camera.h) Camera.y = Camera.h; */

	EnemyBird[0]->UpdateEnemy();
}

void Window::Render()
{
	SDL_RenderClear(renderer);
	level->RenderLevel();

	EnemyBird[0]->RenderEnemy();
	manager.Draw();
	SDL_RenderPresent(renderer);
}

void Window::RunGame(int i_, int endAt_, int milliseconds_)
{
	while (i_ < endAt_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "This game has loaded! Now go!\n";
		++i_;
	}
	std::cout << "We're done looping RunGame() thread!\n";
}

void Window::Level1(int i_, int endAt_, int milliseconds_)
{
	while (i_ < endAt_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "You are at Level 1! Go now!\n";
		++i_;
	}
	std::cout << "We're done looping Level1 thread!\n";
}

void Window::PlayerAt(int i_, int endAt_, int milliseconds_)
{
	while (i_ < endAt_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));

		std::cout << "Player is at (" << PlayerBird.AddComponent<PlayerTransformComponent>().position.x << ", " <<
			PlayerBird.AddComponent<PlayerTransformComponent>().position.y << ")\n";
		++i_;
	}
	std::cout << "We're done looping player position thread!\n";
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
