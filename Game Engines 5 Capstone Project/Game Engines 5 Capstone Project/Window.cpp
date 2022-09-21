#include "Window.h"
#include <iostream>

SDL_Texture* playerBird;
SDL_Rect* srcRect, destRect;

Window::Window(const char* name, int x, int y, int w, int h, bool fullScreen)
{
	int flags = 0;

	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow(name, x, y, w, h, flags);
	renderer = SDL_CreateRenderer(window, -1, 0);

	isRunning = true;

	SDL_Surface* playerSurface = IMG_Load("Flappy bird sprite.png");

	playerBird = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);
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
	// By the way you can modify the size of the sprite here
	destRect.h = 180;
	destRect.w = 300;
}

void Window::Render()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, playerBird, NULL, &destRect);
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
