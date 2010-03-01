#ifndef RANDOM_H_
#define RANDOM_H_

#include <boost/noncopyable.hpp>
#include <boost/ref.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_real.hpp>
#include <cassert>


// Handles frame timings for the game.
class Random: boost::noncopyable {
public:

	Random();
    
	// Returns true or false, randomly.
	static bool boolean();

	// Returns an integer on [0, max).
	template<typename T>
	static T integer(T max) {
		return integer(0, max);
	}

	// Returns an integer on [min, max).
	template<typename T>
	static T integer(T min, T max) {
		return current().generate(boost::uniform_int<T>(min, max - 1));
	}
	
	// Returns a real on [0, 1).
	static double real();

	// Returns a real on [0, max).
	static double real(double max) {
		return real(0, max);
	}

	// Returns a real on [min, max).
	static double real(double min, double max);

	static Random& current() { assert(_current); return *_current; }
	static void setCurrent(Random *current) { _current = current; }

private:
	template<typename Distribution>
	typename Distribution::result_type generate(Distribution d)
	{
		return d(_engine);
	}
	
	typedef boost::mt19937 Engine;
	Engine _engine;

	static Random *_current;
};



#endif
