#include "CreditsMenu.h"

SDL_Event CreditsMenu::creditsEvent;

CreditsMenu::CreditsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    // Initialize window when the start button is pressed
    creditsWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);

    creditsRenderer = SDL_CreateRenderer(creditsWindow, -1, SDL_RENDERER_ACCELERATED);

    creditsSurface = IMG_Load("Credits.png");
    creditsTexture = SDL_CreateTextureFromSurface(creditsRenderer, creditsSurface);

    memoryPool = new MemoryPool();

    threadPool.Start(10);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

CreditsMenu::~CreditsMenu()
{
}

void CreditsMenu::CreditsMenuHandleEvents()
{
    while (SDL_PollEvent(&creditsEvent)) {

        switch (creditsEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (creditsEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                isRunning = false;
                break;
            }

        default:
            break;
        }
    }
}

void CreditsMenu::RenderCreditsMenu()
{
    SDL_RenderClear(creditsRenderer);
    SDL_RenderCopy(creditsRenderer, creditsTexture, NULL, NULL);
    SDL_RenderPresent(creditsRenderer);
}

void CreditsMenu::Clear()
{
    SDL_DestroyWindow(creditsWindow);
    SDL_DestroyRenderer(creditsRenderer);
    SDL_Quit();
}

bool CreditsMenu::Running()
{
    return isRunning;
}
