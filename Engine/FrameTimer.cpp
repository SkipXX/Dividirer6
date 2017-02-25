#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
{
	last = steady_clock::now();
}

double FrameTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<double> frameTime = last - old;
	return frameTime.count();
}
