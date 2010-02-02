#ifndef FRAME_TIMER_H_
#define FRAME_TIMER_H_

#include <boost/noncopyable.hpp>
#include <RakNet/GetTime.h>
#include <cassert>
#include <deque>

// Handles frame timings for the game.
class FrameTimer: boost::noncopyable {
public:

    FrameTimer();
    
	// Sleeps until the next frame and updates the frame timer.
	void step();
	
	// Gets the absolute current time, in seconds.
	double now() const;
	
	// Gets the time of the current frame, in seconds.
	double frameTime() const { return _frameTime; }
	
	// Gets the elapsed time between this frame and the previous, in seconds.
	double elapsed() const { return _elapsed; }
	
	// Changes the target framerate of the simulation.
	void setFramerate(double rate) { assert(rate > 0); _framerate = rate; }
	double framerate() const { return _framerate; }
	
	// The actual framerate experienced
	double actualFramerate() const;
	
	// Convert to/from a RakNet timestamp
	static RakNetTime toTimestamp(double time) { return RakNetTime(time * 1000); }
	static double fromTimestamp(RakNetTime timestamp) { return double(timestamp) / 1000; }
	
	// Current global object.
	static void setCurrent(FrameTimer *current) { _current = current; }
	static FrameTimer& current() { assert(_current); return *_current; }

private:
	typedef std::deque<double> FrameTiming;
	FrameTiming _frameTiming;
	double _totalFrameTiming;
	
	double _frameTime;
	double _elapsed;
	double _framerate;
	static FrameTimer *_current;
};



#endif
