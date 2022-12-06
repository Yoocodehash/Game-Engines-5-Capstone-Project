#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>
#include <SDL_opengl.h>
#include <vulkan/vulkan.hpp>
#include "Button.h"
#include "ThreadPool.h"
#include "Memory.h"
#include "Audio.h"
#include "Profiler.h"

class LevelCompleteScreen
{
public:
	LevelCompleteScreen(const char* title_, int x_, int y_, int w_, int h_, int flags_);
	~LevelCompleteScreen();

	void UpdateLevelCompleteScreen();
	void HandleEventLevelCompleteScreen();
	void RenderLevelCompleteScreen();

	void Clear();
	bool Running();

	ThreadPool threadPool;
	MemoryPool* memoryPool;

	static SDL_Event LevelCompleteEvent;
	Button Exit;
	bool isRunning;

private:
	SDL_Window* LevelCompleteWindow;
	SDL_Renderer* LevelCompleteRenderer;
	SDL_Surface* LevelCompleteSurface;
	SDL_Texture* LevelCompleteTexture;
	Audio ButtonSound;
	Profiler* profiler;
	Mouse mouse;
};