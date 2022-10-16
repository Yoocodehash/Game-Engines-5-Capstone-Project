#include "Window.h"
#include "Memory.h"
#include "Threadpool.h"
#include "Audio.h"

#include <iostream>
#include <thread>
#include "Button.h"

Window* window;
Memory* memory;
ThreadPool threadPool;
MemoryPool* memoryPool;
Audio *audio;

SDL_Window* win = SDL_CreateWindow("Main Menu", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);

SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

double delta = .0001;
double time2 = SDL_GetTicks();

int main(int argc, char **argv) 
{
	// Initialize everything here
	memory = new Memory({ "Enemy Flappy Bird.png", "Flappy bird sprite.png"});
	memoryPool = new MemoryPool();
	audio = new Audio();

    Mouse mouse;

    //SDL_Rect rect{ 150, 70, 100, 200 };
    //SDL_Rect rect2{ 0, 1, 65, 154 };
    //float x, y;

    //x = rect.x;
    //y = rect.y;

    Button startButton;
    startButton.srect.y = 0;
    startButton.drect.x = 1280 / 2 - startButton.drect.w / 2;
    startButton.drect.y = 200;

    Button exitButton;
    exitButton.srect.y = 700;
    exitButton.drect.x = 1280 / 2 - startButton.drect.w / 2;
    exitButton.drect.y = 300;

    audio->LoadAudio(); // Load music before the window opens
    threadPool.Start(10);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    while (true) {

        mouse.update();
        startButton.update(mouse);
        exitButton.update(mouse);

        delta = (SDL_GetTicks() - time2) / 1000;
        time2 = SDL_GetTicks();

        // event loop
        SDL_Event e;
        while (SDL_PollEvent(&e)) {

            switch (e.type)
            {
            case SDL_QUIT:
                return false;
                break;

            case SDL_KEYUP:
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) 
                {
                    return false;
                }

            case SDL_MOUSEBUTTONUP:

                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (startButton.isSelected)
                    {
                        // Destroy Main Menu window
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
                                Mix_PlayMusic(audio->music, -1);
                            }

                            window->HandleEvents();
                            window->Update();
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
                        memory->TypeNumbers(3);

                        return 0;
                    }

                    // If you choose to exit the main menu, the above functions won't show up in the console at all
                    if (exitButton.isSelected) 
                    {
                        return false;
                        break;
                    }
                }

            }
        }

        //rect.x = x;

        SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 100, 0, 100, 255);
        SDL_RenderFillRect(ren, 0);
        SDL_SetRenderDrawColor(ren, 200, 0, 0, 255);
        SDL_RenderDrawRect(ren, 0);
        startButton.draw();
        exitButton.draw();
        mouse.draw();
        SDL_RenderPresent(ren);
    }	
}