#pragma once

#include <stdio.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <new>
#include <string>
#include <cstddef>
#include <thread>

class MemoryPool {
public:
	MemoryPool();
	MemoryPool(const std::size_t elementSize_, const std::size_t elements_, const std::size_t aligmnent_);

	MemoryPool(const MemoryPool& alloc) = delete;
	MemoryPool& operator=(const MemoryPool& rhs) = delete;
	MemoryPool(MemoryPool&& alloc) = delete;
	MemoryPool& operator=(MemoryPool&& rhs) = delete;

	~MemoryPool();

	friend std::ostream& operator<<(std::ostream& os, MemoryPool& pool);

	void* AllocateUnaligned(const std::size_t size_bytes);
	void FreeUnaligned(void* pmemory);

	void* AllocateAligned(const  std::size_t size_bytes, const std::size_t alignment);
	void  FreeAligned(void* pmemory);

	void AllocatePool(const std::size_t elementSize_, const std::size_t elements_, const std::size_t aligmnent_);

	void* GetElement();
	void  FreeElement(void* pelement);

	std::size_t	GetElementSize() const;
	void  ReleaseMemoryPool();
private:
	std::size_t poolSize; //the size in bytes of the pool
	std::size_t elementSize; // size in bytes of each element
	std::size_t alignment; // the element/pool alignment requirement in bytes
	void* memory; // pointer to the first address of the pool, used to relase all the memory
	void** freeMemory; // pointer to pointer, used to point to the head of the free list
};

class Memory
{
public:

	Memory(std::vector<const char*> file_);
	~Memory();

private:
	std::vector<const char*> file;

};

