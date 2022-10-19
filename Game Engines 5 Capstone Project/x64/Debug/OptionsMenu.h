#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "ThreadPool.h"
#include "Memory.h"

class OptionsMenu
{
public:
    OptionsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_);
    ~OptionsMenu();

    void OptionsMenuHandleEvents();
    void RenderOptionsMenu();
    void Clear();

    bool Running();

    static SDL_Event optionsEvent;

private:
    SDL_Window* optionsWindow;
    SDL_Renderer* optionsRenderer;
    ThreadPool threadPool;
    MemoryPool* memoryPool;
    bool isRunning;
};

