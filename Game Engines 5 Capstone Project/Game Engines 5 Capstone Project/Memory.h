#pragma once

#include <stdio.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <new>
#include <string>

struct Vec4
{
	float x, y, z, w;
};

using byte = unsigned char;

/*struct MonitorMemory {
	std::size_t size;
	bool isFreed;
	struct MonitorMemoryTMP* next;
	struct MonitorMemoryTMP* previous;
}; */

struct Vec3
{
	float x, y, z;
};

struct Vec2
{
	float x, y;
};

class Memory
{
public:

	Memory(std::vector<const char*> file_, std::vector<std::size_t> playerSize_);
	~Memory();

	void MemorySizeOfVectors();
	void ManageMemory(int max, char startLetterAt);
	void TypeNumbers(int maxAmount);

private:
	std::vector<const char*> File;
	unsigned long amountAllocated = 0;
	static Memory* instance;
	byte* file;
	std::size_t fileIndex;

};

