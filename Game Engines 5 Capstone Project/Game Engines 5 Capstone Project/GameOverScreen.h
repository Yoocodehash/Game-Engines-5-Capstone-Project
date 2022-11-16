#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include "Button.h"
#include <vulkan/vulkan.h>
#include "ThreadPool.h"
#include "Memory.h"
#include "OptionsMenu.h"
#include "Audio.h"

class GameOverScreen
{
public:
	GameOverScreen(const char* title_, int x_, int y_, int w_, int h_, int flags_);
	virtual ~GameOverScreen();

	void UpdateGameOver();
	void HandleEventGameOver();
	void RenderGameOver();

	void Clear();
	bool Running();

	static SDL_Event gameoverEvent;
	Button Quit;
	bool isRunning;
	//Audio GameOverButtonsSound;

	ThreadPool threadPool;
	MemoryPool* memoryPool;

private:
	SDL_Window* gameoverWindow;
	SDL_Renderer* gameoverRenderer;
	SDL_Surface* gameoverSurface;
	SDL_Texture* gameoverTexture;
	Mouse mouse;
};