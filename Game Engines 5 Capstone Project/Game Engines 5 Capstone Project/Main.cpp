#include "Window.h"

#include "Multithreading.h"
#include "MemoryAllocation.h"

Window* window;
Multithreading threading;
MemoryAllocation memory;

int main(int argc, char **argv) {

	// Initialize SDL window here
	window = new Window("Flying Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	// These will run before the window opens to keep things simple
	//threading.Run(50, 500);
	//memory.ReturnMemoryAllocation();

	// While the window is running, handle events, update and render the whole window
	while (window->Running())
	{
		window->HandleEvents();
		window->Update();
		window->Render();
	}

	return 0;
}