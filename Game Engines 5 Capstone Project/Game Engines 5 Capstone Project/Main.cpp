#include "Window.h"
#include "Memory.h"
#include "Threadpool.h"
#include "Audio.h"

#include <iostream>
#include <thread>

Window* window;
Memory* memory;
ThreadPool threadPool;
MemoryPool* memoryPool;
Audio *audio;

int main(int argc, char **argv) 
{
	// Initialize SDL window here
	window = new Window("Flying Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
	memory = new Memory({ "Enemy Flappy Bird.png", "Flappy bird sprite.png"});
	memoryPool = new MemoryPool();
	audio = new Audio();

	audio->LoadAudio(); // Load music before the window opens
	threadPool.Start(10);
	memoryPool->AllocatePool(10, 10, 1);
	memoryPool->AllocateAligned(13, 1);

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