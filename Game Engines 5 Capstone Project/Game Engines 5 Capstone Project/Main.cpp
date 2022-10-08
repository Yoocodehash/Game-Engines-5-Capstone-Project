#include "Window.h"
#include "Memory.h"
#include "Threadpool.h"

#include <iostream>
#include <thread>

Window* window;
Memory* memory;
ThreadPool threadPool;
MemoryPool* memoryPool;

int main(int argc, char **argv) 
{
	// Initialize SDL window here
	window = new Window("Flying Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
	memory = new Memory({ "Enemy Flappy Bird.png", "Flappy bird sprite.png"});
	memoryPool = new MemoryPool();

	threadPool.Start(10);
	memoryPool->AllocatePool(10, 10, 1);
	memoryPool->AllocateAligned(13, 1);

	// While the window is running, handle events, update and render the whole window
	while (window->Running())
	{
		window->HandleEvents();
		window->Update();
		window->Render();
	}

	if (!window->Running()) // If the window is not running, then close and destroy the window
	{
		window->Clear();
		threadPool.Abort();
	}

	// The typing minigame will be done after the window has been closed (along with the thread/memory pool)
	threadPool.Finish();
	memoryPool->ReleaseMemoryPool();
	memory->~Memory();
	memory->TypeNumbers(3);

	return 0;
}