#include "random.h"
#include <ctime>

Random *Random::_current = 0;

Random::Random()
{
    _engine.seed(unsigned(std::time(0)));
}

bool Random::boolean() {
	return (current().generate(boost::uniform_smallint<int>(0, 1)) != 0);
}

double Random::real() {
	return current().generate(boost::uniform_01<double>());
}

double Random::real(double min, double max) {
	return current().generate(boost::uniform_real<double>(min, max));
}
