#include "Multithreading.h"

Multithreading* multiThread = new Multithreading();

Multithreading::Multithreading()
{

}

Multithreading::~Multithreading()
{

}

// Run multithreading here
void Multithreading::Run(int i_, int milliseconds_) {
	bool isRunning = true;
	static long long frameCount = 0;

	// (function, object pointer, starting point, end point, milliseconds (used for std::chrono))
	std::thread thread1(&Multithreading::Level, multiThread, 0, 20, 1000);
	thread1.detach();

	std::thread thread2(&Multithreading::PlayerAt, multiThread, 0, 20, 1000);
	thread2.detach();

	// (function, object pointer, maxNumber, milliseconds)
	std::thread thread3(&Multithreading::multiThreading, multiThread, 3, 500);
	thread3.detach();

	// Loop isRunning as many times as I want (in main.cpp)
	while (isRunning) {
		if (frameCount == i_) {
			isRunning = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << frameCount << " load this game!\n";
		frameCount++;
	}


}

void Multithreading::multiThreading(int i_, int milliseconds_)
{
	for (int number = 0; number < i_; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "Multi-threading is now shown on the console, as now this 1st thread is about Scott's love for master loops\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "1st thread is now complete! Scott has stopped master looping threads!\n";

	for (int number = 0; number < i_; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "2nd thread here is about how The World Is Yours (reference to Scarface)!\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "2nd thread is now complete! So the world might not be yours anymore!\n";

	for (int number = 0; number < i_; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "3rd thread here is where The Last Of Us 2 got Game Of The Year Award in 2020\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "3rd thread is now complete! No more The Last Of Us 2!\n";

	for (int number = 0; number < i_; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "4th thread here is about COVID vaccines becoming a thing\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "4th thread is now complete! The world might be free of coronavirus!\n";


	for (int number = 0; number < i_; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << "5th thread here is about Microsoft developing Windows 11 and everyone is loving it!\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "5th thread is now complete! Microsoft might get rid of Windows 11 and make Windows 12!\n";
}



void Multithreading::Level(int i_, int endAt_, int milliseconds_)
{
	while (i_ < endAt_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << i_ << "% loading level...\n";
		++i_;
	}
	std::cout << "Level loaded!\n";
}

void Multithreading::PlayerAt(int i_, int endAt_, int milliseconds_)
{
	while (i_ < endAt_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_));
		std::cout << i_ << "% loading player...\n";
		++i_;
	}
	std::cout << "Player loaded!\n";
}