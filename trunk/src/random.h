#ifndef RANDOM_H_
#define RANDOM_H_

#include <boost/noncopyable.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_real.hpp>
#include <cassert>


// Handles frame timings for the game.
class Random: boost::noncopyable {
public:

	Random() { }
    
	// Returns true or false, randomly.
	static bool boolean() {
		return (current().generate(boost::uniform_smallint<int>(0, 1)) != 0);
	}

	// Returns an integer on [0, max).
	template<typename T>
	static T integer(T max) {
		return current().generate(boost::uniform_int<T>(0, max - 1));
	}

	// Returns an integer on [min, max).
	template<typename T>
	static T integer(T min, T max) {
		return current().generate(boost::uniform_int<T>(min, max - 1));
	}
	
	// Returns a real on [0, 1).
	template<typename T>
	static T real() {
		return current().generate(boost::uniform_01<T>());
	}

	// Returns a real on [0, max).
	template<typename T>
	static T real(T max) {
		return current().generate(boost::uniform_real<T>(0, max));
	}

	// Returns a real on [min, max).
	template<typename T>
	static T real(T min, T max) {
		return current().generate(boost::uniform_real<T>(min, max));
	}

	static Random& current() { assert(_current); return *_current; }
	static void setCurrent(Random *current) { _current = current; }

private:
	template<typename Distribution>
	typename Distribution::result_type generate(const Distribution& d)
	{
		typedef boost::variate_generator<Engine, Distribution> Generator;
		return Generator(_engine, d)();
	}
	
	typedef boost::mt19937 Engine;
	Engine _engine;

	static Random *_current;
};



#endif
