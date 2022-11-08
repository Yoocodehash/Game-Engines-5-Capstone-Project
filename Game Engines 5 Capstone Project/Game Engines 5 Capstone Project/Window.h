#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "PauseMenu.h"
#include "LevelCompleteScreen.h"
#include "GameOverScreen.h"
#include "Audio.h"

class ColliderComponent;

class Window
{

public:

	Window(const char *title_, int x, int y, int w, int h, bool fullScreen);
	~Window();

	void HandleEvents();
	void Update();
	void InFrustum(); // Returns true if the player is within the camera frustum
	void ShowLevelCompleteScreen();
	void ShowLevelFailedScreen();
	void Render();
	void Clear();

	bool Running();

	void GamePaused();
	void GameUnpaused();

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static SDL_Rect Camera;
	static std::vector<ColliderComponent*> colliders;
	bool isPaused;
private:
	bool isRunning;
	SDL_Window* window;
	PauseMenu* PauseGame;
	LevelCompleteScreen* LevelFinished;
	GameOverScreen* LevelFailed;
	Audio anyAudio;
};