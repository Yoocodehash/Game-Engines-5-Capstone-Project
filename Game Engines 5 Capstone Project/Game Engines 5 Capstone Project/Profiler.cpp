#include "Profiler.h"
#include <SDL_timer.h>

__int64 Profiler::frequency = 0;

Profiler::Profiler()
{
	if (frequency == 0)
	{
		frequency = SDL_GetPerformanceFrequency();
	}

	started_at = SDL_GetPerformanceFrequency();
}


void Profiler::Start(std::string functionName_)
{
	/* Provides the time origin, and current time in sub - millisecond resolution,
	such that it is not subject to system clock skew or adjustments */

	started_at = SDL_GetPerformanceCounter();

	std::cout << functionName_ << " profiler current value of the high resolution counter: " 
		<< started_at << std::endl;
}

double Profiler::GetMilliseconds(std::string functionName_) const
{
	// Calculate and return the milliseconds

	double ms = 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));

	std::cout << functionName_ << " profiler milliseconds: " << ms << " ms" << std::endl;

	return ms;
}

__int64 Profiler::GetTicks(std::string functionName_) const
{
	// Get Ticks calls every frame

	uint64_t ticks = SDL_GetPerformanceCounter() - started_at;

	std::cout << functionName_ << " profiler ticks: " << ticks << std::endl;

	return ticks;
}