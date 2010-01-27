#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <cmath>

// A mostly-immutable 2-dimensional vector

struct Vector2D {
	Vector2D(): x(), y() { }
	Vector2D(double x, double y): x(x), y(y) { }
	
	// Returns the length squared of this vector (faster than length()).
	double lengthSquared() const;
	// Returns the length of this vector.
	double length() const;
	// Returns a vector pointing in the same direction, of length 1.
	Vector2D normalized() const;
	
	// Returns the dot product of this and another vector.
	// |a||b|cos(theta)
	double dot(const Vector2D& rhs) const;
	// Returns the perpendicular dot product of this and another vector.
	// Similar to cross product, or wedge product.
	// |a||b|sin(theta)
	double perpDot(const Vector2D& rhs) const;
	
	// x and y coordinates
	double x;
	double y;
};

// Negation
inline Vector2D operator+(const Vector2D& v)
{
	return v;
}
inline Vector2D operator-(const Vector2D& v)
{
	return Vector2D(-v.x, -v.y);
}

// Vector addition/subtraction
inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x + v2.x, v1.y + v2.y);
}
inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x - v2.x, v1.y - v2.y);
}
inline Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
{
	v1 = v1 + v2;
	return v1;
}
inline Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
{
	v1 = v1 - v2;
	return v1;
}

// Scalar multiplication/division
inline Vector2D operator*(const Vector2D& v, double scalar)
{
	return Vector2D(v.x * scalar, v.y * scalar);
}
inline Vector2D operator*(double scalar, const Vector2D& v)
{
	return v * scalar;
}
inline Vector2D operator/(const Vector2D& v, double scalar)
{
	return Vector2D(v.x / scalar, v.y / scalar);
}
inline Vector2D operator/(double scalar, const Vector2D& v)
{
	return Vector2D(scalar / v.x, scalar / v.y);
}

inline Vector2D& operator*=(Vector2D& v, double scalar)
{
	v = v * scalar;
	return v;
}
inline Vector2D& operator/=(Vector2D& v, double scalar)
{
	v = v / scalar;
	return v;
}


inline bool operator==(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x == v2.x && v1.y == v2.y);
}
inline bool operator!=(const Vector2D& v1, const Vector2D& v2)
{
	return !(v1 == v2);
}

inline double Vector2D::lengthSquared() const
{
	return this->dot(*this);
}
inline double Vector2D::length() const
{
	return std::sqrt(lengthSquared());
}
inline Vector2D Vector2D::normalized() const
{
	return *this / length();
}

inline double Vector2D::dot(const Vector2D& rhs) const
{
	return x * rhs.x + y * rhs.y;
}
inline double Vector2D::perpDot(const Vector2D& rhs) const
{
	return x * rhs.y - y * rhs.x;
}

// Not working for some reason.
/*
namespace std {

// Returns the minimal/maximal coordinates of the pair.
inline ::Vector2D min(const ::Vector2D& v1, const ::Vector2D& v2)
{
	return ::Vector2D(min(v1.x, v2.x), min(v1.y, v2.y));
}
inline ::Vector2D max(const ::Vector2D& v1, const ::Vector2D& v2)
{
	return ::Vector2D(max(v1.x, v2.x), max(v1.y, v2.y));
}

}
*/
#endif
