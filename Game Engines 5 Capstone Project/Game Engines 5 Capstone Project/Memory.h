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

// Defines the new operator to pass in the file of the code and the line in the code
// Note: this define statement is needed to actually run the allocation, deallocation and memory leaks
// Without it, there will be no print statements to the console on where the allocation, deallocation 
// and memory leaks occur
#define new new (__FILE__, __LINE__)

/* These need to be extern because it will cause errors in the .cpp file where the non-static booleans
need to be a reference to a specific object */

// The define functions need to be here so that we can use it in Main.cpp and run the memory monitor properly
extern bool traceMemoryOn;
#define TraceMemoryOn() traceMemoryOn = true // Make the boolean true if trace memory is on
#define TraceMemoryOff() traceMemoryOn = false // Make the boolean false if trace memory is off

extern bool activeMemoryOn;
#define ActiveMemoryOn() activeMemoryOn = true // Make the boolean true if active memory is on
#define ActiveMemoryOff() activeMemoryOn = false // Make the boolean false if active memory is off

// Create a struct that holds the memory map info (doesn't matter if you put in here or .cpp file, it works either way)
struct MemoryMapInfo 
{
	void* pointer;
	const char* file;
	long line;
};
