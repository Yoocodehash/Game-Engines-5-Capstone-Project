#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <vulkan/vulkan.h>
#include "Button.h"
#include "ThreadPool.h"
#include "Memory.h"
#include "OptionsMenu.h"
#include "Audio.h"

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

	static SDL_Event pauseEvent;
	Button Quit;
	bool isRunning;
	Audio PauseMenuButtonsSound;

	ThreadPool threadPool;
	MemoryPool* memoryPool;

private:
	SDL_Window* pauseWindow;
	SDL_Renderer* pauseRenderer;
	SDL_Surface* pauseSurface;
	SDL_Texture* pauseTexture;
	Button Resume, Options;
	Mouse mouse;
	OptionsMenu* optionsMenu;
};

