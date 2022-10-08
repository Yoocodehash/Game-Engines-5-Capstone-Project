#include "Window.h"
#include "TextureManager.h"
#include "Level.h"
#include <iostream>
#include "Components.h"
#include "Enemy.h"
#include <Windows.h>
#include <Xinput.h>

PlayerTransformComponent transform;
Controller* gameController = nullptr;

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
