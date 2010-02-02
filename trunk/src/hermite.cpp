#include "hermite.h"
#include <cassert>

HermiteInterpolation::HermiteInterpolation()
: _initialTime()
, _finalTime()
, _initialPosition()
, _finalPosition()
, _initialVelocity()
, _finalVelocity()
{
}

HermiteInterpolation::HermiteInterpolation(double initialTime, double finalTime,
	const Vector2D& initialPosition, const Vector2D& finalPosition,
	const Vector2D& initialVelocity, const Vector2D& finalVelocity)
: _initialTime(initialTime)
, _finalTime(finalTime)
, _initialPosition(initialPosition)
, _finalPosition(finalPosition)
, _initialVelocity(initialVelocity)
, _finalVelocity(finalVelocity)
{
}

Vector2D HermiteInterpolation::interpolatePosition(double t) const
{
	assert(t >= _initialTime);
	if (t >= _finalTime) {
		return _finalPosition + _finalVelocity * (t - _finalTime);
	}
	
	t = (t - _initialTime) / (_finalTime - _initialTime);
	assert(t >= 0 && t < 1);
	//return (1 - t) * _initialPosition + t * _finalPosition;

	double t_2 = t * t;
	double t_3 = t * t_2;

	//double h00 =  2 * t_3 - 3 * t_2 + 1;
	double h10 =      t_3 - 2 * t_2 + t;
	//double h01 = -2 * t_3 - 3 * t_2;
	double h11 =      t_3 -     t_2;

	double time = _finalTime - _initialTime;

	return (1 - t) * _initialPosition +
	       h10 * time * _initialVelocity +
	       t * _finalPosition +
	       h11 * time * _finalVelocity;
}

Vector2D HermiteInterpolation::interpolateVelocity(double t) const
{
	assert(t >= _initialTime);
	if (t >= _finalTime) {
		return _finalVelocity;
	}
	 return _finalVelocity;
	/*
	t = (t - _initialTime) / (_finalTime - _initialTime);
	assert(t >= 0 && t < 1);

	double t_2 = t * t;

	//double h00 =  6 * t_2 - 6 * t;
	double h10 =  3 * t_2 - 4 * t + 1;
	//double h01 = -6 * t_2 - 6 * t;
	double h11 =  3 * t_2 - 2 * t;

	double time = _finalTime - _initialTime;

	return	-_initialPosition +
		h10 * time * _initialVelocity +
		_finalPosition +
		h11 * time * _finalVelocity;
	*/
}
