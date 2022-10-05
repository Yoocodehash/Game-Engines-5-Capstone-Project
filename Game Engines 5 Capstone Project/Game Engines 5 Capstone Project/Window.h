#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <list>

class Window
{

public:
	Window(const char *title_, int x, int y, int w, int h, bool fullScreen);
	~Window();

	void RunMultithread();
	void HandleEvents();
	void Update();
	void Render();
	void RunGame(int i_, int endAt_, int milliseconds_);
	void Level1(int i_, int endAt_, int milliseconds_);
	void PlayerAt(int i_, int endAt_, int milliseconds_);
	void Clear();

	bool Running();

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static SDL_Rect Camera;
private:
	bool isRunning;
	SDL_Window* window;
	Window* ourWindow;
};