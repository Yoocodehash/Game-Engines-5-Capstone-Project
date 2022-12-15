#include "Profiler.h"
#include <SDL_timer.h>
#include <chrono>

__int64 Profiler::frequency = 0;


Profiler::Profiler()
{
	if (frequency == 0)
	{
		frequency = SDL_GetPerformanceFrequency();
	}

	started_at = SDL_GetPerformanceFrequency();

	cpuUsage = -1;
	lastRun = 0;
	runCount = 0;

	ZeroMemory(&prevSystemKernel, sizeof(FILETIME));
	ZeroMemory(&prevSystemUser, sizeof(FILETIME));

	ZeroMemory(&prevProcessKernel, sizeof(FILETIME));
	ZeroMemory(&prevProcessUser, sizeof(FILETIME));
}


void Profiler::Start(std::string functionName_)
{
	/* Provides the time origin, and current time in sub - millisecond resolution,
	such that it is not subject to system clock skew or adjustments */

	started_at = SDL_GetPerformanceCounter();
}

double Profiler::GetMilliseconds(std::string functionName_) const
{
	// Calculate and return the milliseconds

	double ms = 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));

	std::cout << functionName_ << " profiler milliseconds: " << ms << " ms" << std::endl;

	return ms;
}

short Profiler::GetCPUusage()
{
	//create a local copy to protect against race conditions in setting the member variable
	short localCPU = cpuUsage;

	if (::InterlockedIncrement(&runCount) == 1)
	{

		/* If this is called too often, the measurement itself will greatly affect the results
		meaning that the numbers might be too big or a negative integer */

		if (!EnoughTimePassed())
		{
			::InterlockedDecrement(&runCount);
			return localCPU;
		}
	}

	FILETIME SystemIdle, SystemKernel, SystemUser;
	FILETIME ProcessCreation, ProcessExit, ProcessKernel, ProcessUser;

	if (!GetSystemTimes(&SystemIdle, &SystemKernel, &SystemUser) ||
		!GetProcessTimes(GetCurrentProcess(), &ProcessCreation, &ProcessExit, &ProcessKernel, &ProcessUser))
	{
		::InterlockedDecrement(&runCount);
		return localCPU;
	}

	if (!IsFirstRun())
	{
		/* CPU usage is calculated by getting the total amount of time the system has operated since the
		last measurement (made up of kernel + user) and the total amount of time the process has run
		(kernel + user)  */

		ULONGLONG SystemKernelDiff = SubtractTimes(SystemKernel, prevSystemKernel);
		ULONGLONG SystemUserDiff = SubtractTimes(SystemUser, prevSystemUser);

		ULONGLONG ProcessKernelDiff = SubtractTimes(ProcessKernel, prevProcessKernel);
		ULONGLONG ProcessUserDiff = SubtractTimes(ProcessUser, prevProcessUser);

		ULONGLONG totalSystem = SystemKernelDiff + SystemUserDiff;
		ULONGLONG totalProcess = ProcessKernelDiff + ProcessUserDiff;

		if (totalSystem > 0)
		{
			cpuUsage = (short)((100.0 * totalProcess) / totalSystem);
		}
	}

	prevSystemKernel = SystemKernel;
	prevSystemUser = SystemUser;
	prevProcessKernel = ProcessKernel;
	prevProcessUser = ProcessUser;

	lastRun = GetTickCount64();

	localCPU = cpuUsage;

	::InterlockedDecrement(&runCount); // Yeah we wrote it, we just took notes of this tutorial
	// we're folling on how to print cpu usage with the process kernel and the system user kernel

	return localCPU;
}

ULONGLONG Profiler::SubtractTimes(const FILETIME& a_, const FILETIME& b_)
{
	LARGE_INTEGER a, b;
	a.LowPart = a_.dwLowDateTime; // This substracts the times for the cpu usage from now to before
	a.HighPart = a_.dwHighDateTime;

	b.LowPart = b_.dwLowDateTime;
	b.HighPart = b_.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}

bool Profiler::EnoughTimePassed()
{
	const int minimumElapsedTimeInMilliseconds = 250; // This function is where the time has
	// passed for a minimun of 250 milliseconds, the cpu usage will be able to be returned

	ULONGLONG currentTickCount = GetTickCount64();
	return (currentTickCount - lastRun) > minimumElapsedTimeInMilliseconds;
}

int Profiler::GetMemoryUsage()
{
	// Get the handle to this process
	auto myHandle = GetCurrentProcess();

	// Fill in the process' memory usage details
	PROCESS_MEMORY_COUNTERS pmc;

	// Return the usage in megabytes
	if (GetProcessMemoryInfo(myHandle, &pmc, sizeof(pmc)))
		return(pmc.WorkingSetSize / 1000000);
	else
		return 0;
}