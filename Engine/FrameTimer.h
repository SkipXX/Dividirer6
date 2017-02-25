#pragma once
#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	double Mark();
private:
	std::chrono::steady_clock::time_point last;
};