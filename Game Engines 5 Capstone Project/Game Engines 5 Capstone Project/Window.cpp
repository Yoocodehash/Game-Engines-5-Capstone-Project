#include "Window.h"
#include "TextureManager.h"
#include "Level.h"
#include "GameObject.h"
#include <iostream>

GameObject* playerBird;
SDL_Rect* srcRect, destRect;
Level* level;

SDL_Renderer* Window::renderer = nullptr;

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

	playerBird = new GameObject("Flappy bird sprite.png", -50, 200);

	level = new Level();
}

Window::~Window()
{

}

void Window::HandleEvents()
{
	SDL_Event event;

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
	playerBird->UpdatePlayer();
}

void Window::Render()
{
	SDL_RenderClear(renderer);
	level->RenderLevel();
	playerBird->Render();
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
