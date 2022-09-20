#include "Multithreading.h"
#include "MemoryAllocation.h"

Multithreading threading;
MemoryAllocation memory;

int main() {

	threading.Run(50, 500);
	memory.ReturnMemoryAllocation();

	return 0;
}