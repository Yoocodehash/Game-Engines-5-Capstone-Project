#include "OptionsMenu.h"

SDL_Event OptionsMenu::optionsEvent;

OptionsMenu::OptionsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    // Initialize window when the start button is pressed
    optionsWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    optionsRenderer = SDL_CreateRenderer(optionsWindow, -1, SDL_RENDERER_ACCELERATED);

    memoryPool = new MemoryPool();

    exitButton.srect.y = 710;
    exitButton.drect.x = 800 / 2 - exitButton.drect.w / 2;
    exitButton.drect.y = 350;

    threadPool.Start(10);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::UpdateOptionsMenu()
{
    mouse.update();
    exitButton.update(mouse);
}

void OptionsMenu::OptionsMenuHandleEvents()
{
    while (SDL_PollEvent(&optionsEvent)) {

        switch (optionsEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (optionsEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                isRunning = false;
                break;
            }

        case SDL_MOUSEBUTTONUP:
                if (optionsEvent.button.button == SDL_BUTTON_LEFT)
                {
                    if (exitButton.isSelected)
                    {
                        threadPool.Finish();
                        memoryPool->ReleaseMemoryPool();

                        isRunning = false;
                        break;
                    }
                }

        default:
            break;
        }
    }
}

void OptionsMenu::RenderOptionsMenu()
{
    SDL_SetRenderDrawColor(optionsRenderer, 50, 50, 50, 255);
    SDL_RenderClear(optionsRenderer);
    SDL_SetRenderDrawColor(optionsRenderer, 100, 0, 100, 255);
    SDL_RenderFillRect(optionsRenderer, 0);
    SDL_SetRenderDrawColor(optionsRenderer, 200, 0, 0, 255);
    SDL_RenderDrawRect(optionsRenderer, 0);
    exitButton.draw();
    mouse.draw();
    SDL_RenderPresent(optionsRenderer);
}

void OptionsMenu::Clear()
{
    SDL_DestroyWindow(optionsWindow);
    SDL_DestroyRenderer(optionsRenderer);
    SDL_Quit();
}

bool OptionsMenu::Running()
{
    return isRunning;
}