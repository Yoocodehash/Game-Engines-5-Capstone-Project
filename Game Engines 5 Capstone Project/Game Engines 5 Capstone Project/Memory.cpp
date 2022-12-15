#include "Memory.h"


MemoryManagement::MemoryManagement()
{
}


MemoryManagement::~MemoryManagement()
{
}

// This is where the string pointer allocates the memory if it doesn't equal to nullptr
void MemoryManagement::WriteStrings()
{
    /* Create 2 int variables, one that stores the max size of strings you want to type(in this case, MaxSize)
    And the other stores the amount of total input strings (not the actual string size, but how many
    strings you are ABOUT to type (in this case, Number) */

    int MaxSize, Number;
    std::string* stringPointer; // Create a string pointer to allocate the memory of typed input strings

    // Type a number on how many strings you want to type
    std::cout << "How many strings do you want to type? ";
    std::cin >> MaxSize;

    // Initialize the string pointer to be the max size of the typed string
    stringPointer = new std::string[MaxSize];

    // If the memory cannot be allocated, then return this error statement to the console
    if (stringPointer == nullptr)
    {
        std::cerr << "The memory string could not be allocated\n";
    }

    /* Else you can enter a string you want to input and it'll be received as soon as the number variable 
    is equal to the max size variable */
    else
    {
        std::cout << "\n"; // This is for formatting and makes the strings more legible

        for (Number = 0; Number < MaxSize; Number++)
        {
            std::cout << "Enter a string: ";
            std::cin >> stringPointer[Number];
        }

        std::cout << "\nReceived string: ";
        for (Number = 0; Number < MaxSize; Number++)
        {
            std::cout << stringPointer[Number] << "\n";
        }

        // Delete the string pointer and print this statement to the console that the string pointer has been freed
        delete[] stringPointer;

        std::cout << "\nThe string pointer has been freed and deallocated\n\n";
    }
}

MemoryMonitor::MemoryMonitor()
{
}

MemoryMonitor::~MemoryMonitor()
{
}

void MemoryMonitor::MonitorMemory()
{
    //std::cout << "Allocating " << size_ << " bytes of memory\n";
    //std::allocator<MemoryMonitor> mem;
}