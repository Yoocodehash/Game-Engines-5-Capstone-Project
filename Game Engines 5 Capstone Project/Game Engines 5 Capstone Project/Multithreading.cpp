#include "Multithreading.h"

/// Kind of like the GameManager
void Multithreading::Run() {
	bool isRunning = true;
	static long long frameCount = 0;
	std::thread thread1(&DoSomethingCool);
	thread1.detach();

	//thread1.join(); /// Whats the difference between detach and join?
	std::thread thread2(&DoSomethingElse, 12);
	thread2.detach();

	std::thread thread3(&MultiThreading);
	thread3.detach();

	/// This represents the master game loop
	while (isRunning) {
		if (frameCount == 200) {
			isRunning = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << frameCount << " master loop()\n";
		frameCount++;
	}


}

void Multithreading::MultiThreading()
{
	for (int number = 0; number < 3; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "Multi-threading is now shown on the console, as now this 1st thread is about Scott's love for master loops\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "1st thread is now complete! Scott has stopped master looping threads!\n";

	for (int number = 0; number < 3; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "2nd thread here is about how The World Is Yours (reference to Scarface)!\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "2nd thread is now complete! So the world might not be yours anymore!\n";

	for (int number = 0; number < 3; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "3rd thread here is where The Last Of Us 2 got Game Of The Year Award in 2020\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "3rd thread is now complete! No more The Last Of Us 2!\n";

	for (int number = 0; number < 3; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "4th thread here is about COVID vaccines becoming a thing\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "4th thread is now complete! The world might be free of coronavirus!\n";


	for (int number = 0; number < 3; number++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "5th thread here is about Microsoft developing Windows 11 and everyone is loving it!\n";

		if (number == 3)
		{
			break;
		}
	}
	std::cout << "5th thread is now complete! Microsoft might get rid of Windows 11 and make Windows 12!\n";
}



void Multithreading::DoSomethingCool() {
	int i = 0;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << i << " Hello from doSomethingCool()\n";
		++i;
		if (i == 20) {
			break; /// break out of the loop
		}
	}
	std::cout << i << " doSomethingCool() is done\n";
}

void Multithreading::DoSomethingElse(int data_)
{
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << data_ << " Hello from doSomethingElse()\n";
		++data_;
		if (data_ == 20) {
			break; /// break out of the loop
		}
	}
	std::cout << data_ << " doSomethingElse() is done\n";
}