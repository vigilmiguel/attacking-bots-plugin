#ifndef TICK_H
#define TICK_H

#include <chrono>

using namespace std::chrono;



static long long getCurrentTick()
{
	milliseconds time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());


	return time.count();
}


#endif