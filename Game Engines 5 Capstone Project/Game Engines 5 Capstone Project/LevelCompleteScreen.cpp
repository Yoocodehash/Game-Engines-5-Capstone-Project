#include "LevelCompleteScreen.h"

SDL_Event LevelCompleteScreen::LevelCompleteEvent;

LevelCompleteScreen::LevelCompleteScreen(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    // Initialize window when pause menu is pressed
    LevelCompleteWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    LevelCompleteRenderer = SDL_CreateRenderer(LevelCompleteWindow, -1, SDL_RENDERER_ACCELERATED);

    LevelCompleteSurface = IMG_Load("Level Complete.png");
    LevelCompleteTexture = SDL_CreateTextureFromSurface(LevelCompleteRenderer, LevelCompleteSurface);

    Exit.srect.y = 710;
    Exit.drect.x = 100 / 2 - Exit.drect.w / 2;
    Exit.drect.y = 450;

    memoryPool = new MemoryPool();

    ButtonSound.LoadAudio();

    threadPool.Start(10);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

LevelCompleteScreen::~LevelCompleteScreen()
{
}

void LevelCompleteScreen::UpdateLevelCompleteScreen()
{
    mouse.update();
    Exit.update(mouse);
}

void LevelCompleteScreen::HandleEventLevelCompleteScreen()
{
    while (SDL_PollEvent(&LevelCompleteEvent)) {

        switch (LevelCompleteEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (LevelCompleteEvent.key.keysym.sym)
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

void LevelCompleteScreen::RenderLevelCompleteScreen()
{
    SDL_RenderClear(LevelCompleteRenderer);
    SDL_RenderCopy(LevelCompleteRenderer, LevelCompleteTexture, NULL, NULL);
    Exit.draw();
    mouse.draw();
    SDL_RenderPresent(LevelCompleteRenderer);
}

void LevelCompleteScreen::Clear()
{
    // Don't use SDL_Quit() because it'll quit the whole window program
    SDL_DestroyWindow(LevelCompleteWindow);
    SDL_DestroyRenderer(LevelCompleteRenderer);
    SDL_Quit();
}

bool LevelCompleteScreen::Running()
{
    return isRunning;
}
