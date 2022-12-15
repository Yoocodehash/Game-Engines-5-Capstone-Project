#pragma once

#include <stdio.h>
#include <iostream>
#include <map>

class MemoryManagement
{
public:

	MemoryManagement();
	~MemoryManagement();

	void WriteStrings(); // This is where the string pointer allocates the memory if it doesn't equal to nullptr
};

class MemoryMonitor
{
public:

	MemoryMonitor();
	~MemoryMonitor();

	void MonitorMemory();
};