#pragma once

#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <windows.h>
#include <Psapi.h>

class Profiler
{
public:
	Profiler();

	void Start(std::string functionName_);
	double GetMilliseconds(std::string functionName_) const;

	short GetCPUusage();
	int GetMemoryUsage();

private:
	__int64 started_at;
	static __int64 frequency;

	short cpuUsage;

	ULONGLONG SubtractTimes(const FILETIME& a_, const FILETIME& b_);
	bool EnoughTimePassed();
	inline bool IsFirstRun() const { return (lastRun == 0); }

	//system total times
	FILETIME prevSystemKernel;
	FILETIME prevSystemUser;

	//process times
	FILETIME prevProcessKernel;
	FILETIME prevProcessUser;

	ULONGLONG lastRun;
	volatile LONG runCount;
};