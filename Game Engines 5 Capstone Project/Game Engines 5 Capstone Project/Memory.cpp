#include "Memory.h"

#undef new /* This is needed to define the new operator for the memory monitor (without it, it'll say that new is
undefined and give an error */


MemoryManagement::MemoryManagement()
{
}


MemoryManagement::~MemoryManagement()
{
}

// This is where the string pointer allocates the memory if it doesn't equal to nullpointer
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

// The rest of these variables and functions below are for memory monitor and doesn't need a class for it

// Initialize the global booleans from Memory.h
bool traceMemoryOn = true;
bool activeMemoryOn = false;

// These variables need to be initialized in .cpp to prevent definer errors (happens only when it's put in the .h file)
// Create and initialize the memory map data to the max amount of pointers
const size_t MaxPointers = 10000u;
MemoryMapInfo MemoryMap[MaxPointers];
size_t NullPointerSize = 0;

// Searches the map for an address
int SearchMapForAddress(void* pointer_) 
{
    /* For i is less than null pointer size, increment i
    and if the memory map pointer equal to the parameter pointer, return i */

    for (size_t i = 0; i < NullPointerSize; ++i)
    {
        if (MemoryMap[i].pointer == pointer_)
        {
            return i;
        }
    }

    // If not, return -1 (error)
    return -1;
}

// Delete the memory map pointer
void DeleteMemoryPointer(void* pointer_) 
{
    // Local variable to store the number for the memory map pointer
    int pos = SearchMapForAddress(pointer_);

    // Remove pointer from memory map using the local variable
    for (size_t i = pos; i < NullPointerSize - 1; ++i)
    {
        MemoryMap[i] = MemoryMap[i + 1];
        --NullPointerSize;
    }
}

// Find memory leaks after the pointer is deleted
void SearchForMemoryLeaks()
{
    // If the null pointer size is greater than 0, then the memory has leaked and print where the leak occurs at
    if (NullPointerSize > 0)
    {
        std::cout << "\nLeaked memory at: ";
        for (size_t i = 0; i < NullPointerSize; ++i)
        {
            std::cout << MemoryMap[i].pointer << " (File: " << MemoryMap[i].file << ", on line " <<
                MemoryMap[i].line << ")\n";
        }
    }

    // Else if it's equal to or less than 0, then print to the console that there's no memory leaks
    else
    {
        std::cout << "No memory leaks!\n";
    }
};

// Overload the operator new only for a single object pointer
void* operator new(size_t size_, const char* file_, long line_) 
{
    // Initialize the pointer to be equal to the malloc of the size of the pointer
    void* pointer = malloc(size_);

    // If active memory is turned on, initialize the memory map pointer, file and line of code
    if (activeMemoryOn) 
    {
        /* If the null pointer size is equal to the max pointers, then print to the console that the memory map
        is a nullptr */

        if (NullPointerSize == MaxPointers) 
        {
            std::cout << "The memory map is a nullptr\n";
        }

        // Make the memory map array variables equal to the local pointer variable and the parameters passed in
        MemoryMap[NullPointerSize].pointer = pointer;
        MemoryMap[NullPointerSize].file = file_;
        MemoryMap[NullPointerSize].line = line_;

        // Increment the null pointer size so that the memory map is not equal to null
        ++NullPointerSize;
    }

    // If trace memory is turned on, then print the memory allocation at a particular address and line of code
    if (traceMemoryOn) 
    {
        std::cout << "\nAllocated " << size_ << " bytes at address " << pointer <<
            " (File: " << file_ << ", on line " << line_ << ")\n";
    }

    // Return the local pointer variable
    return pointer;
}

// Overload the operator new for multiple object pointers which will pass in the size, code file, and the line of code
void* operator new[](size_t size_, const char* file_, long line_) 
{
    // Return operator new(size_, file_, line_)
    return operator new(size_, file_, line_);
}

// Override the operator delete for a single object pointer which only passes in the void pointer variable
void operator delete(void* pointer_) 
{
    // If the map address is greater than 0, then free the memory map and delete it
    if (SearchMapForAddress(pointer_) >= 0)
    {
        free(pointer_);
        DeleteMemoryPointer(pointer_);

        // If trace memory is on, print the deleted memory and search for memory leaks after
        if (traceMemoryOn)
        {
            std::cout << "\nDeleted memory at address " << pointer_ << "\n";

            // After the pointers are deleted, search for memory leaks (only works properly for array delete operators)
            SearchForMemoryLeaks();
        }

    }

    // Else if there's an unknown pointer somewhere, then print to the console that the pointer will be deleted
    else if (!pointer_ && activeMemoryOn)
    {
        std::cout << "\nAttempt to delete unknown pointer: " << pointer_ << "\n";
    }
}

// Overload the operator delete for multiple object pointers
void operator delete[](void* pointer_) 
{
    // Use the operator delete function above
    operator delete(pointer_);
}