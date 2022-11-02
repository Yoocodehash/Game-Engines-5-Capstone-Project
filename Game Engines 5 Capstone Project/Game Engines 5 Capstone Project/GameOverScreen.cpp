#include "GameOverScreen.h"

SDL_Event GameOverScreen::gameoverEvent;

GameOverScreen::GameOverScreen(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    // Initialize window when pause menu is pressed
    gameoverWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    gameoverRenderer = SDL_CreateRenderer(gameoverWindow, -1, SDL_RENDERER_ACCELERATED);

    gameoverSurface = IMG_Load("Game Over.png");
    gameoverTexture = SDL_CreateTextureFromSurface(gameoverRenderer, gameoverSurface);

    Quit.srect.y = 710;
    Quit.drect.x = 800 / 2 - Quit.drect.w / 2;
    Quit.drect.y = 450;

    memoryPool = new MemoryPool();

    //GameOverButtonsSound.LoadAudio();

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::UpdateGameOver()
{
    mouse.update();
    Quit.update(mouse);
}

void GameOverScreen::HandleEventGameOver()
{
    while (SDL_PollEvent(&gameoverEvent)) {

        switch (gameoverEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (gameoverEvent.key.keysym.sym)
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

void GameOverScreen::RenderGameOver()
{
    SDL_RenderClear(gameoverRenderer);
    SDL_RenderCopy(gameoverRenderer, gameoverTexture, NULL, NULL);
    Quit.draw();
    mouse.draw();
    SDL_RenderPresent(gameoverRenderer);
}

void GameOverScreen::Clear()
{
    SDL_DestroyWindow(gameoverWindow);
    SDL_DestroyRenderer(gameoverRenderer);
}

bool GameOverScreen::Running()
{
    return isRunning;
}