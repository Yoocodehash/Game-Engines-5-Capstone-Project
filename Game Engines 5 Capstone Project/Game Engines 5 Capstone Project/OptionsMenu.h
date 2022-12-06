#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_vulkan.h>
#include <SDL_opengl.h>
#include <vulkan/vulkan.h>
#include <string>
#include "ThreadPool.h"
#include "Memory.h"
#include "Profiler.h"
#include "Button.h"

class OptionsMenu
{
public:
    OptionsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_);
    ~OptionsMenu();

    void UpdateOptionsMenu();
    void OptionsMenuHandleEvents();
    void RenderOptionsMenu();
    void Clear();

    bool Running();

    static SDL_Event optionsEvent;

private:
    Mouse mouse;
    Button exitButton;
    SDL_Window* optionsWindow;
    SDL_Renderer* optionsRenderer;
    SDL_Surface* optionsSurface;
    SDL_Texture* optionsTexture;
    ThreadPool threadPool;
    MemoryPool* memoryPool;
    Profiler* profiler;
    bool isRunning;
};

