#include "Window.h"
#include "Memory.h"
#include "Threadpool.h"
#include "Audio.h"
#include "OptionsMenu.h"
#include "CreditsMenu.h"

#include <iostream>
#include <thread>
#include "Button.h"

Window* window;
Memory* memory;
ThreadPool threadPool;
MemoryPool* memoryPool;
Audio *audio;
OptionsMenu* optionsMenu;
CreditsMenu* creditsMenu;

SDL_Window* win = SDL_CreateWindow("Main Menu", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

SDL_Surface* mainMenuSurface = IMG_Load("Main Menu.png");
SDL_Texture* mainMenuTexture = SDL_CreateTextureFromSurface(ren, mainMenuSurface);

double delta = .0001;
double time2 = SDL_GetTicks();

int main(int argc, char **argv) 
{
	// Initialize memory pool and audio here
	memoryPool = new MemoryPool();

    Mouse mouse;

    //SDL_Rect rect{ 150, 70, 100, 200 };
    //SDL_Rect rect2{ 0, 1, 65, 154 };
    //float x, y;

    //x = rect.x;
    //y = rect.y;

    Button startButton;
    startButton.srect.y = 10;
    startButton.drect.x = 800 / 2 - startButton.drect.w / 2;
    startButton.drect.y = 50; // Will place the button according to the screen's position Y axis

    Button optionButton;
    optionButton.srect.y = 310; // This will give me the button name depending on the button's position
    optionButton.drect.x = 800 / 2 - optionButton.drect.w / 2;
    optionButton.drect.y = 150;

    Button creditsButton;
    creditsButton.srect.y = 410;
    creditsButton.drect.x = 800 / 2 - creditsButton.drect.w / 2;
    creditsButton.drect.y = 250;

    Button backButton;
    backButton.srect.y = 200;
    backButton.drect.x = 800 / 2 - backButton.drect.w / 2;
    backButton.drect.y = 200;

    Button exitButton;
    exitButton.srect.y = 710;
    exitButton.drect.x = 800 / 2 - exitButton.drect.w / 2;
    exitButton.drect.y = 350;

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    // Initialize memory files here
    memory = new Memory({ "Enemy Flappy Bird.png", "Flappy bird sprite.png", "Buttons.png",
        "Credits.png", "Game Over.png", "Level 1.png", "Level Complete.png", "Main Menu.png", "Options Menu.png",
        "Pause Menu.png" });

    audio = new Audio();
    audio->LoadAudio(); // Load music before the window opens

    while (true) 
    {
        // If music isn't playing
        if (Mix_PlayingMusic() == 0)
        {
            //Play the music
            Mix_PlayMusic(audio->MainMenuMusic, -1);
        }

        mouse.update();
        startButton.update(mouse);
        optionButton.update(mouse);
        creditsButton.update(mouse);
        backButton.update(mouse);
        exitButton.update(mouse);

        delta = (SDL_GetTicks() - time2) / 1000;
        time2 = SDL_GetTicks();

        // event loop
        SDL_Event e;
        while (SDL_PollEvent(&e)) {

            switch (e.type)
            {
            case SDL_QUIT:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                return false;
                break;

            case SDL_KEYUP:
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) 
                {
                    threadPool.Finish();
                    memoryPool->ReleaseMemoryPool();

                    return false;
                }

            case SDL_MOUSEBUTTONUP:

                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (startButton.isSelected)
                    {
                        //Stop the music
                        Mix_HaltMusic();

                        Mix_PlayChannel(-1, audio->ButtonPressedSound, 0);

                        // Destroy Main Menu renderer and window
                        SDL_DestroyRenderer(ren);
                        SDL_DestroyWindow(win);

                        // Initialize window when the start button is pressed
                        window = new Window("Flying Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

                        // While the window is running, handle events, update and render the whole window
                        while (window->Running())
                        {

                            // If music isn't playing
                            if (Mix_PlayingMusic() == 0)
                            {
                                //Play the music
                                Mix_PlayMusic(audio->GameMusic, -1);
                            }

                            window->HandleEvents();
                            window->Update();
                            window->ShowLevelCompleteScreen();
                            window->ShowLevelFailedScreen();
                            window->Render();
                        }

                        if (!window->Running()) // If the window is not running, then close and destroy the window
                        {
                            window->Clear();
                            threadPool.Abort();

                            audio->DestroyAudio();
                        }

                        // The typing minigame will be done after the window has been closed (along with the thread/memory pool)
                        threadPool.Finish();
                        memoryPool->ReleaseMemoryPool();
                        memory->~Memory();

                        return 0;
                    }

                    if (optionButton.isSelected)
                    {
                        Mix_PlayChannel(-1, audio->ButtonPressedSound, 0);

                        // Destroy Main Menu renderer and window
                        SDL_DestroyRenderer(ren);
                        SDL_DestroyWindow(win);

                        // Initialize window when the start button is pressed
                        optionsMenu = new OptionsMenu("Options", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 800, 600, 0);


                        while (optionsMenu->Running())
                        {
                            optionsMenu->UpdateOptionsMenu();
                            optionsMenu->OptionsMenuHandleEvents();
                            optionsMenu->RenderOptionsMenu();
                        }

                        if (!optionsMenu->Running())
                        {
                            optionsMenu->Clear();
                        }

                        return 0;
                    }

                    if (creditsButton.isSelected)
                    {
                        Mix_PlayChannel(-1, audio->ButtonPressedSound, 0);

                        // Destroy Main Menu renderer and window
                        SDL_DestroyRenderer(ren);
                        SDL_DestroyWindow(win);

                        // Initialize window when the start button is pressed
                        creditsMenu = new CreditsMenu("Flying Bird Credits", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 800, 600, IMG_INIT_PNG);

                        while (creditsMenu->Running())
                        {
                            // If music isn't playing
                            if (Mix_PlayingMusic() == 0)
                            {
                                //Play the music
                                Mix_PlayMusic(audio->CreditsMusic, 0);
                            }

                            creditsMenu->CreditsMenuHandleEvents();
                            creditsMenu->RenderCreditsMenu();
                        }

                        if (!creditsMenu->Running())
                        {
                            creditsMenu->Clear();
                            audio->DestroyAudio();

                        }

                        return 0;
                    }

                    // If you choose to exit the main menu, the above functions won't show up in the console at all
                    if (exitButton.isSelected) 
                    {
                        threadPool.Finish();
                        memoryPool->ReleaseMemoryPool();
                        audio->DestroyAudio();

                        return false;
                        break;
                    }
                }

            }
        }

        //rect.x = x;

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, mainMenuTexture, NULL, NULL);
        startButton.draw();
        optionButton.draw();
        creditsButton.draw();
        exitButton.draw();
        mouse.draw();
        SDL_RenderPresent(ren);
    }

    return 0;
}