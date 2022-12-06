#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <vulkan/vulkan.h>
#include <string>
#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "PauseMenu.h"
#include "LevelCompleteScreen.h"
#include "GameOverScreen.h"
#include "Profiler.h"
#include "Audio.h"
#include "ParticleEffects.h"

class ColliderComponent;

class Window
{

public:
	Window(const char* title_, int x, int y, int w, int h, int flags_);
	~Window();

	void HandleEvents(int flags_);
	void Update();
	void InFrustum(); // Returns true if the player is within the camera frustum
	void ShowLevelCompleteScreen(int flags_);
	void ShowLevelFailedScreen(int flags_);
	void Render();
	void Clear();

	bool Running();

	void GamePaused();
	void GameUnpaused();

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static SDL_Rect Camera;
	bool isPaused;
private:
	int ScreenWidth, ScreenHeight;
	bool isRunning;
	SDL_Window* window;
	PauseMenu* PauseGame;
	ParticleEffects* particle;
	LevelCompleteScreen* LevelFinished;
	Profiler* profiler;
	Audio anyAudio;
};