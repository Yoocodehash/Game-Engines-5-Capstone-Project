#include "Memory.h"

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <iostream>

MemoryPool::MemoryPool() : poolSize(0), elementSize(0), alignment(0), memory(nullptr), freeMemory(nullptr) {}


MemoryPool::MemoryPool(const std::size_t elementSize_, const std::size_t elements_, const std::size_t alignment_)
{
	memory = freeMemory = nullptr;
	AllocatePool(elementSize_, elements_, alignment_);
	
	assert(elementSize >= sizeof(void*));
	assert(elementSize % alignment == 0);
	assert((alignment & (alignment - 1)) == 0);

	poolSize = (elementSize * elements_) + alignment_;
	memory = AllocateAligned(elementSize * elements_, alignment);

	if (memory == nullptr) 
	{
		std::cerr << "There is an error with  " << __FUNCTION__ << ": Unable to allocate/Out of memory! " << std::endl;
	}

	else 
	{
		freeMemory = static_cast<void**>(memory);
		
		std::uintptr_t endAddress = reinterpret_cast<std::uintptr_t>(freeMemory) + (elementSize * elements_);

		for (std::size_t elementCount = 0; elementCount <  elements_; ++elementCount) 
		{
			std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t>(freeMemory) + elementCount * elementSize;
			std::uintptr_t nextAddress = currentAddress + elementSize;
			
			void **currentMemory = reinterpret_cast<void**>(currentAddress);

			if(nextAddress >= endAddress) 
			{
				*currentMemory = nullptr;
			}

			else 
			{
				*currentMemory = reinterpret_cast<void*>(nextAddress);
			}
		}
	}
}


void MemoryPool::AllocatePool(const std::size_t elementSize_, const std::size_t elements_, const std::size_t alignment_)
{
	assert(memory == nullptr);

	elementSize = elementSize_;
	alignment = alignment_;

	assert(elementSize >= sizeof(void*));
	assert(elementSize % alignment == 0);
	assert((alignment & (alignment - 1)) == 0);

	poolSize = (elementSize * elements_) + alignment_;

	memory = AllocateAligned(elementSize * elements_, alignment);
	if (memory == nullptr) 
	{
		std::cerr << "ERROR  " << __FUNCTION__ << ": Unable to allocate " << std::endl;
	}

	else 
	{
		freeMemory = static_cast<void**>(memory);
		
		std::uintptr_t endAddress = reinterpret_cast<std::uintptr_t>(freeMemory) + (elementSize * elements_);

		for (std::size_t elementCount = 0; elementCount < elements_; ++elementCount) 
		{
			std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t>(freeMemory) + elementCount * elementSize;
			std::uintptr_t nextAddress = currentAddress + elementSize;
			
			void** currentMemory = reinterpret_cast<void**>(currentAddress);

			if (nextAddress >= endAddress) 
			{
				*currentMemory = nullptr;
			}

			else 
			{
				*currentMemory = reinterpret_cast<void*>(nextAddress);
			}
		}
	}
}

void* MemoryPool::GetElement()
{
	if (memory == nullptr) {
		std::cerr << "ERROR " << __FUNCTION__ << ": No memory was allocated to this pool" << std::endl;
		return nullptr;
	}

	if (freeMemory != nullptr) 
	{
		void* block = reinterpret_cast<void*>(freeMemory);
		freeMemory = static_cast<void**>(*freeMemory);

		return block;
	}

	else 
	{
		std::cerr << "ERROR " << __FUNCTION__ << ": out of memory blocks" << std::endl;
		return nullptr;
	}
}

void MemoryPool::FreeElement(void* pblock)
{
	if (pblock == nullptr) 
	{
		return;
	}

	if (memory == nullptr) 
	{
		std::cerr << "ERROR " << __FUNCTION__ << ": No memory was allocated to this pool" << std::endl;
		return;
	}

	if (freeMemory == nullptr) 
	{
		freeMemory = reinterpret_cast<void**>(pblock);
		*freeMemory = nullptr;
	}

	else 
	{
		void** ppreturned_block = freeMemory;
		freeMemory = reinterpret_cast<void**>(pblock);
		*freeMemory = reinterpret_cast<void*>(ppreturned_block);
	}

}


void MemoryPool::ReleaseMemoryPool()
{
	FreeAligned(memory);
	memory = freeMemory = nullptr;

	std::cout << "Memory pool has been released from memory!\n";
}

MemoryPool::~MemoryPool()
{
	ReleaseMemoryPool();
}

void* MemoryPool::AllocateUnaligned(const std::size_t size_)
{
	void* memoryBlock = malloc(size_);

	return memoryBlock;
}

void MemoryPool::FreeUnaligned(void* mem)
{
	free(mem);

	std::cout << "Unaligned memory pool has been freed from memory!\n";
}

void* MemoryPool::AllocateAligned(const  std::size_t size_bytes, const std::size_t alignment_)
{
	assert(alignment_ >= 1);
	assert(alignment_ <= 128);
	assert((alignment_ & (alignment_ - 1)) == 0);

	std::size_t expandedSizeBytes = size_bytes + alignment_;

	std::uintptr_t rawAddress = reinterpret_cast<std::uintptr_t>(AllocateUnaligned(expandedSizeBytes));

	std::size_t mask = alignment_ - 1;
	std::uintptr_t misalignment = rawAddress & mask;
	std::ptrdiff_t adjustment = alignment_ - misalignment;

	std::uintptr_t alignedAddress = rawAddress + adjustment;

	assert(adjustment < 256);
	std::uint8_t* paligned_mem = reinterpret_cast<std::uint8_t*>(alignedAddress);
	paligned_mem[-1] = static_cast<std::uint8_t>(adjustment);

	return static_cast<void*>(paligned_mem);
}

void MemoryPool::FreeAligned(void* mem)
{
	const std::uint8_t* memoryAligned = reinterpret_cast<const std::uint8_t*>(mem);
	std::uintptr_t alignedAddress = reinterpret_cast<std::uintptr_t>(mem);
	std::ptrdiff_t ajustment = static_cast<std::ptrdiff_t>(memoryAligned[-1]);

	std::uintptr_t rawAddress = alignedAddress - ajustment;
	void* rawMemory = reinterpret_cast<void*>(rawAddress);

	FreeUnaligned(rawMemory);

	std::cout << "Aligned memory pool has been freed from memory!\n";
}

std::size_t MemoryPool::GetElementSize() const
{
	return elementSize;
}

std::ostream& operator<<(std::ostream& os, MemoryPool& pool)
{
	if (pool.memory == nullptr) {
		os << "ERROR " << __FUNCTION__ << ": No memory was allocated to this pool" << std::endl;
		return os;
	}

	std::size_t elements_ = (pool.poolSize - pool.alignment) / pool.elementSize;
	os << "Pool memory info: total size of " << pool.poolSize << " bytes| "
		<< pool.elementSize << " byte element size| " << elements_ << " elements| " << pool.alignment << " byte alignment requirement" << std::endl;
	os << "Printing Pool free memory list: "
		<< "memory address | contents(next pointer)" << std::endl;
	void** pplist = pool.freeMemory;
	for (std::size_t elementCount = 0; elementCount < elements_; ++elementCount) {
		if (pplist == nullptr) {
			break;
		}
		
		std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t>(pplist);
		std::uintptr_t nextAddress = reinterpret_cast<std::uintptr_t>(*pplist);
		
		pplist = static_cast<void**>(*pplist);
		os << "\t 0x" << std::hex << currentAddress << " | 0x" << nextAddress << std::endl;
	}

	return os;
}

Memory::Memory(std::vector<const char*> file_)
{
	file = file_;

	std::cout << "\n";
	for (int i = 0; i < file_.size(); i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		std::cout << "File " << file_[i] << " initialized!\n";
	}

	std::cout << "\n";
	for (int i = 0; i < file_.size(); i++)
	{
		std::ifstream in_file(file_[i], std::ios::binary);
		in_file.seekg(0, std::ios::end);

		int fileSize = in_file.tellg(); // Determines the size of the file in bytes

		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		std::cout << "The size of " << file_[i] << " file is " << fileSize << " bytes!\n";
	}
	std::cout << "\n";
}


Memory::~Memory()
{
	std::cout << "\n";
	for (int i = 0; i < file.size(); i++)
	{
		std::cout << "File " << file[i] << " deleted from memory!\n";
	}
}