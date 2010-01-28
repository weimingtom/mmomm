#ifndef HERMITE_H_
#define HERMITE_H_

#include "vector2D.h"

// Keeps track of a hermite interpolation.
class HermiteInterpolation {
public:
	
	HermiteInterpolation();
	HermiteInterpolation(double initialTime, double finalTime,
		const Vector2D& initialPosition, const Vector2D& finalPosition,
		const Vector2D& initialVelocity, const Vector2D& finalVelocity);
	
	// Determine the end of the interpolation
	double finalTime() const { return _finalTime; }
	
	// Interpolate position at the specified time.
	// If past the final time, it will interpolate correctly
	Vector2D interpolatePosition(double t) const;
	
	// Interpolates to velocity at the specified time.
	// If past the final time, it will interpolate correctly
	// Unsure if my derivation is correct
	Vector2D interpolateVelocity(double t) const;

private:
	double _initialTime;
	double _finalTime;
	Vector2D _initialPosition;
	Vector2D _finalPosition;
	Vector2D _initialVelocity;
	Vector2D _finalVelocity;
	
};

#endif

