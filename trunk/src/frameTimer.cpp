
#include "frameTimer.h"
#include <RakNet/GetTime.h>
#include <RakNet/RakSleep.h>

FrameTimer *FrameTimer::_current = 0;

FrameTimer::FrameTimer()
:	_frameTime(now())
,	_elapsed(0)
,	_framerate(60)
{
}

void FrameTimer::step()
{
	double timeToSleep = 1 / _framerate;
	timeToSleep -= now() - _frameTime;
	if (timeToSleep < 0)
		timeToSleep = 0;
	
	// Always sleep, even if 0, to play nice with other programs
	RakSleep(static_cast<unsigned int>(toTimestamp(timeToSleep)));
	
	double previous = _frameTime;
	_frameTime = now();
	_elapsed = _frameTime - previous;
}

// Gets the absolute current time, in seconds.
double FrameTimer::now() const
{
	return double(RakNet::GetTimeUS()) / (1000 * 1000);
}

