#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <list>

class Multithreading
{
public:

	Multithreading();
	~Multithreading();

	void Run(int i_, int milliseconds_);

private: 
	
	// Makes the multithreading functions only accessible to this class
	void multiThreading(int i_, int milliseconds_);
	void Level(int i_, int endAt_, int milliseconds_);
	void PlayerAt(int i_, int endAt_, int milliseconds_);
};

