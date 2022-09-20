#include "Multithreading.h"
#include "MemoryAllocation.h"

Multithreading threading;
MemoryAllocation mem;

int main() {

	threading.Run();
	mem.ReturnMemoryAllocation();
	return 0;
}