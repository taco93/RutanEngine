#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetElapsedTime()
{
	if (isRunning) {
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);

		return elapsed.count();
	}
	else {
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart()
{
	this->isRunning = true;
	this->start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!this->isRunning) return false;

	stop = std::chrono::high_resolution_clock::now();
	this->isRunning = false;
	return true;
}

bool Timer::Start()
{
	if (this->isRunning) return false;

	this->start = std::chrono::high_resolution_clock::now();
	this->isRunning = true;
	return true;
}
