#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <string>
#include <set>
#include "ThreadPool.h"
#include "Profiler.h"

class CreditsMenu
{
public:
    CreditsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_);
    ~CreditsMenu();

    void CreditsMenuHandleEvents();
    void RenderCreditsMenu();
    void Clear();

    bool Running();

    static SDL_Event creditsEvent;
private:
    SDL_Window* creditsWindow;
    SDL_Renderer* creditsRenderer;
    SDL_Surface* creditsSurface;
    SDL_Texture* creditsTexture;

    Profiler* profiler;

    ThreadPool threadPool;

    bool isRunning;
};