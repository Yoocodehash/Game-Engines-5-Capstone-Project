#pragma once

#include <stdio.h>
#include <iostream>
#include <cstdint>

class Profiler
{
public:
	Profiler();

	void Start(std::string functionName_);
	double GetMilliseconds(std::string functionName_) const;
	__int64 GetTicks(std::string functionName_) const;

private:
	__int64 started_at;
	static __int64 frequency;
};