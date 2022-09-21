#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Window
{

public:
	Window(const char *title_, int x, int y, int w, int h, bool fullScreen);
	~Window();

	void HandleEvents();
	void Update();
	void Render();
	void Clear();

	bool Running();
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
};