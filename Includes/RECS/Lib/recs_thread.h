#pragma once
#include <thread>

/*
	Wrapper class for a thread.
	
*/
class recs_thread
{
private:

	std::thread* m_thread;

public:


};

/*
	This is a thread pool class that is recommended
	to use if continually creating and destroying threads is degrading performance.
*/
class recs_thread_pool
{
private:

	std::thread* m_threads;

public:


};