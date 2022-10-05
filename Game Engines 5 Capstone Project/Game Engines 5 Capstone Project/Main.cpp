#include "Window.h"
#include "Memory.h"

Window* window;
Memory* memory;

int main(int argc, char **argv) 
{

	// Initialize SDL window here
	window = new Window("Flying Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
	memory = new Memory({ "Enemy Flappy Bird.png", "Flappy bird sprite.png"}, 10);

	// While the window is running, handle events, update and render the whole window
	while (window->Running())
	{
		window->HandleEvents();
		window->Update();
		window->Render();
		//window->RunMultithread(); // has exception errors
	}

	if (!window->Running()) // If the window is not running, then close and destroy the window
	{
		window->Clear();
	}

	// The typing minigame will be done after the window has been closed
	memory->~Memory();
	memory->MemorySizeOfVectors();
	memory->ManageMemory(13, 'c');
	memory->TypeNumbers(3);

	return 0;
}