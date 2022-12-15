#include "OptionsMenu.h"

SDL_Event OptionsMenu::optionsEvent;

OptionsMenu::OptionsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    optionsWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    optionsRenderer = SDL_CreateRenderer(optionsWindow, -1, SDL_RENDERER_ACCELERATED);

    optionsSurface = IMG_Load("Options Menu.png");
    optionsTexture = SDL_CreateTextureFromSurface(optionsRenderer, optionsSurface);

    threadPool.Start(1);

    exitButton.srect.y = 710;
    exitButton.drect.x = 800 / 2 - exitButton.drect.w / 2;
    exitButton.drect.y = 550; // This is where the button is and knows that it's clicked
    // once you hit the button

    profiler = new Profiler(); // We got CPU usage and memory usage running inside the options menu

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

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (optionsEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();

                isRunning = false;
                break;
            }

        case SDL_MOUSEBUTTONUP:
                if (optionsEvent.button.button == SDL_BUTTON_LEFT)
                {
                    if (exitButton.isSelected)
                    {
                        threadPool.Finish();

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
    profiler->Start("OptionsMenu::RenderOptionsMenu()");
    profiler->GetMilliseconds("OptionsMenu::RenderOptionsMenu()");

    printf("Thread id %d: OptionsMenu::RenderOptionsMenu() CPU usage: %d%%\n", ::GetCurrentThreadId(),
        profiler->GetCPUusage());

    printf("Thread id %d: OptionsMenu::RenderOptionsMenu() memory usage: %i MB\n", ::GetCurrentThreadId(),
        profiler->GetMemoryUsage());

    SDL_RenderClear(optionsRenderer);
    SDL_RenderCopy(optionsRenderer, optionsTexture, NULL, NULL);
    exitButton.draw();
    mouse.draw();
    SDL_RenderPresent(optionsRenderer);
}

void OptionsMenu::Clear()
{
    // Don't use SDL_Quit() because it'll quit the whole window program
    SDL_DestroyWindow(optionsWindow);
    SDL_DestroyRenderer(optionsRenderer);
}

bool OptionsMenu::Running()
{
    return isRunning;
}