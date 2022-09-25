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

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static SDL_Rect Camera;
private:
	bool isRunning;
	SDL_Window* window;
};