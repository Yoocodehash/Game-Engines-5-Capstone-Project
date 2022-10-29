#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Button.h"
#include "ThreadPool.h"
#include "Memory.h"
#include "OptionsMenu.h"

class PauseMenu
{
public:
	PauseMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_);
	virtual ~PauseMenu();

	void UpdatePauseMenu();
	void HandleEventPauseMenu();
	void RenderPauseMenu();

	void Clear();
	bool Running();

private:
	SDL_Window* pauseWindow;
	SDL_Renderer* pauseRenderer;
	Button Options, Quit;
	Mouse mouse;
	SDL_Event pauseEvent;
	OptionsMenu* optionsMenu;
	bool isRunning;
	ThreadPool threadPool;
	MemoryPool* memoryPool;
};

