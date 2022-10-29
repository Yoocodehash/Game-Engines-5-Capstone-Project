#include "PauseMenu.h"

PauseMenu::PauseMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    // Initialize window when pause menu is pressed
    pauseWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    pauseRenderer = SDL_CreateRenderer(pauseWindow, -1, SDL_RENDERER_ACCELERATED);

    Options.srect.y = 310; // This will give me the button name depending on the button's position
    Options.drect.x = 800 / 2 - Options.drect.w / 2;
    Options.drect.y = 150;

    Quit.srect.y = 710;
    Quit.drect.x = 800 / 2 - Quit.drect.w / 2;
    Quit.drect.y = 350;

    memoryPool = new MemoryPool();

    threadPool.Start(10);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    optionsMenu = nullptr;

    isRunning = true;
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::UpdatePauseMenu()
{
    mouse.update();
    Options.update(mouse);
    Quit.update(mouse);
}

void PauseMenu::HandleEventPauseMenu()
{
    while (SDL_PollEvent(&pauseEvent)) {

        switch (pauseEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (pauseEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                isRunning = false;
                break;
            }

        case SDL_MOUSEBUTTONUP:
            if (pauseEvent.button.button == SDL_BUTTON_LEFT)
            {
                if (Options.isSelected)
                {
                    SDL_DestroyRenderer(pauseRenderer);
                    SDL_DestroyWindow(pauseWindow);

                    // Initialize window when the start button is pressed
                    optionsMenu = new OptionsMenu("Options", SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED, 800, 600, 0);

                    while (optionsMenu->Running())
                    {

                        optionsMenu->UpdateOptionsMenu();
                        optionsMenu->OptionsMenuHandleEvents();
                        optionsMenu->RenderOptionsMenu();
                    }
                }
                if (Quit.isSelected)
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

void PauseMenu::RenderPauseMenu()
{
    SDL_SetRenderDrawColor(pauseRenderer, 50, 50, 50, 255);
    SDL_RenderClear(pauseRenderer);
    SDL_SetRenderDrawColor(pauseRenderer, 100, 0, 100, 255);
    SDL_RenderFillRect(pauseRenderer, 0);
    SDL_SetRenderDrawColor(pauseRenderer, 200, 0, 0, 255);
    SDL_RenderDrawRect(pauseRenderer, 0);
    Options.draw();
    Quit.draw();
    mouse.draw();
    SDL_RenderPresent(pauseRenderer);
}

void PauseMenu::Clear()
{
    SDL_DestroyWindow(pauseWindow);
    SDL_DestroyRenderer(pauseRenderer);
}

bool PauseMenu::Running()
{
    return isRunning;
}
