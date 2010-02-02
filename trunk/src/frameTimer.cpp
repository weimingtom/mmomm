
#include "frameTimer.h"
#include <RakNet/GetTime.h>
#include <RakNet/RakSleep.h>

// Average framerate over this many seconds.
const double FRAMERATE_SECONDS = 3;

FrameTimer *FrameTimer::_current = 0;

FrameTimer::FrameTimer()
:	_totalFrameTiming(0)
,	_frameTime(now())
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
	
	// Framerate computation
	if (_totalFrameTiming > FRAMERATE_SECONDS && !_frameTiming.empty()) {
		_totalFrameTiming -= _frameTiming.front();
		_frameTiming.pop_front();
	}
	_totalFrameTiming += _elapsed;
	_frameTiming.push_back(_elapsed);
}

// Gets the absolute current time, in seconds.
double FrameTimer::now() const
{
	return double(RakNet::GetTimeUS()) / (1000 * 1000);
}

double FrameTimer::actualFramerate() const
{
	if (_frameTiming.empty()) {
		return 0;
	}
	return _frameTiming.size() / _totalFrameTiming;
}
