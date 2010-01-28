#include "hermite.h"
#include <cassert>

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

bool HermiteInterpolation::stillInterpolating(double t) const
{
	assert(t > _initialTime);
	return (t < _finalTime);
}

Vector2D HermiteInterpolation::interpolatePosition(double t) const
{
	assert(t >= _initialTime);
	if (t >= _finalTime) {
		return _finalPosition + _finalVelocity * (t - _finalTime);
	}
	
	t = (t - _initialTime) / (_finalTime - _initialTime);
	double a = 1 + 2 * t;
	double b = t - 1;
	double b_2 = b * b;
	double c = 3 - 2 * t;
	double t_2 = t * t;
	return	a * b_2 * _initialPosition +
		t * b_2 * _initialVelocity +
		t_2 * c * _finalPosition +
		t_2 * b * _finalVelocity;
}

Vector2D HermiteInterpolation::interpolateVelocity(double t) const
{
	assert(t >= _initialTime);
	if (t >= _finalTime) {
		return _finalVelocity;
	}
	
	t = (t - _initialTime) / (_finalTime - _initialTime);
	double a = 6 * t;
	double b = t - 1;
	double c = 3 * t - 4;
	double d = 3 * t - 2;
	return	a * b * _initialPosition +
		t * c * _initialVelocity +
		-a * b * _finalPosition +
		t * d * _finalVelocity;
}
