#include "GameOverScreen.h"

SDL_Event GameOver::gameoverEvent;

GameOver::GameOver(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    // Initialize window when pause menu is pressed
    gameoverWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    gameoverRenderer = SDL_CreateRenderer(gameoverWindow, -1, SDL_RENDERER_ACCELERATED);

    gameoverSurface = IMG_Load("Game Over.png");
    gameoverTexture = SDL_CreateTextureFromSurface(gameoverRenderer, gameoverSurface);

    Retry.srect.y = 710;
    Retry.drect.x = 800 / 2 - Retry.drect.w / 2;
    Retry.drect.y = 250;

    ReturnToMainMenu.srect.y = 310; // This will give me the button name depending on the button's position
    ReturnToMainMenu.drect.x = 800 / 2 - ReturnToMainMenu.drect.w / 2;
    ReturnToMainMenu.drect.y = 350;

    memoryPool = new MemoryPool();

    GameOverButtonsSound.LoadAudio();

    threadPool.Start(10);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    gameover = nullptr;

    isRunning = true;
}

GameOver::~GameOver()
{
}

void GameOver::UpdateGameOver()
{
    mouse.update();
    Retry.update(mouse);
    ReturnToMainMenu.update(mouse);
}

void GameOver::HandleEventGameOver()
{
    /*
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

        case SDL_MOUSEBUTTONUP:
            if (gameoverEvent.button.button == SDL_BUTTON_LEFT)
            {
                if (Retry.isSelected)
                {
                    Mix_PlayChannel(-1, OptionsMenuButtonsSound.ButtonPressedSound, 0);

                    threadPool.Finish();
                    memoryPool->ReleaseMemoryPool();

                    isRunning = false;
                    break;
                }

                if (ReturnToMainMenu.isSelected)
                {
                    Mix_PlayChannel(-1, OptionsMenuButtonsSound.ButtonPressedSound, 0);

                    SDL_DestroyRenderer(gameoverRenderer);
                    SDL_DestroyWindow(gameoverWindow);

                    // Initialize window when the start button is pressed
                    gameover = new GameOver("GameOver", SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED, 800, 600, 0);

                    while (gameover->Running())
                    {

                        gameover->UpdateGameOver();
                        gameover->HandleEventGameOver();
                        gameover->RenderGameOver();
                    }

                    if (!gameover->Running())
                    {
                        gameover->Clear();
                        isRunning = false;
                    }

                    break;
                }
            }

        default:
            break;
        }
    }
    */

}

void GameOver::RenderGameOver()
{
    SDL_RenderClear(gameoverRenderer);
    SDL_RenderCopy(gameoverRenderer, gameoverTexture, NULL, NULL);
    Retry.draw();
    ReturnToMainMenu.draw();
    mouse.draw();
    SDL_RenderPresent(gameoverRenderer);
}

void GameOver::Clear()
{
    SDL_DestroyWindow(gameoverWindow);
    SDL_DestroyRenderer(gameoverRenderer);
}

bool GameOver::Running()
{
    return false;
}