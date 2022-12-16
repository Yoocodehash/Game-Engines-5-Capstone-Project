#pragma once

#include <stdio.h>
#include <iostream>
#include <map> // Need to create a memory map and allocate/deallocate it
#include <vector>
#include <cstddef> // For size_t

class MemoryManagement
{
public:

	MemoryManagement();
	~MemoryManagement();

	void WriteStrings(); // This is where the string pointer allocates the memory if it doesn't equal to nullptr
};

// Create the new operators to run the memory monitor
void* operator new(std::size_t, const char*, long);
void* operator new[](std::size_t, const char*, long);

/* Defines the new operator to pass in 2 macros: the file of the code and the line in the code
Note: this define statement is needed to actually run the allocation, deallocation and memory leaks
Without it, there will be no print statements to the console on where the allocation, deallocation and memory leaks occur
new needs to be defined twice in this macro because it's supposed to connect with operator new & new[]
If I only define new once, then there'll be no print statements to the console just like if I were to remove
the define macro completely (that's not necessarily a compliation error, it doesn't know where the new
operator macro is defined in the code */
#define new new(__FILE__, __LINE__)

/* These booleans below need to be extern because it will cause errors in the .cpp file where the booleans are redefined
in Memory.cpp or it'll say this declaration has no storage class or type specifier once I remove the bool
word from both traceMemory and activeMemoryOn in the .cpp file */

/* The define macros need to be here so that we can use it in Main.cpp and run the memory monitor properly
I can't return TraceMemoryOn or ActiveMemoryOn as a function because it causes error in the code
where the TraceMemoryOn or ActiveMemoryOn functions are already defined in Main.obj so therefore we need to
use TraceMemoryOn or ActiveMemoryOn as define macros instead, since it'll work without any compliation errors */

// Trace memory gets the memory from the replacement functions for operator new and operator delete functions
extern bool traceMemoryOn;
#define TraceMemoryOn() { traceMemoryOn = true; } // Make the boolean true if trace memory is on
#define TraceMemoryOff() { traceMemoryOn = false; } // Make the boolean false if trace memory is off

// Active memory gets the current memory allocation information as well as deallocation and memory leaks
extern bool activeMemoryOn;
#define ActiveMemoryOn() { activeMemoryOn = true; } // Make the boolean true if active memory is on
#define ActiveMemoryOff() { activeMemoryOn = false; } // Make the boolean false if active memory is off

// Create a struct that holds the memory map info (doesn't matter if you put in here or .cpp file, it works either way)
struct MemoryMapInfo 
{
	void* pointer;
	const char* file;
	long line;
};