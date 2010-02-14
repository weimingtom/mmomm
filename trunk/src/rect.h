#ifndef RECT_H_
#define RECT_H_

#include <cmath>
#include "vector2D.h"

// Get rid of damn macros
#undef min
#undef max

struct Rect {
	Rect(): left(), top(), right(), bottom() {}
	Rect(const Vector2D& min, const Vector2D& max)
		: left(min.x), top(min.y), right(max.x), bottom(max.y) {}
    Rect(double left, double top, double right, double bottom)
		: left(left), top(top), right(right), bottom(bottom) { }
	
	Vector2D min() const { return Vector2D(left, top); }
	Vector2D max() const { return Vector2D(right, bottom); }
	Vector2D dim() const { return max() - min(); }
	
	// Point containment
	bool contains(const Vector2D& v)
	{
		return contains(Rect(v, v));
	}
	
	// Rectangle containment test.
	bool contains(const Rect& rhs)
	{
		return (left <= rhs.left && right >= rhs.right &&
			top <= rhs.top && bottom >= rhs.bottom);
	}
	
	// Rectangle-rectangle intersection test.
	bool intersects(const Rect& rhs)
	{
		return (left <= rhs.right && right >= rhs.left &&
			top <= rhs.bottom && bottom >= rhs.top);
	}
	
    double left;
    double top;
    double right;
    double bottom;
};

// Vector addition/subtraction
inline Rect operator+(const Rect& r, const Vector2D& v)
{
	return Rect(r.min() + v, r.max() + v);
}
inline Rect operator+(const Vector2D& v, const Rect& r)
{
	return r + v;
}
inline Rect operator-(const Rect& r, const Vector2D& v)
{
	return Rect(r.min() - v, r.max() - v);
}
inline Rect& operator+=(Rect& r, const Vector2D& v)
{
	r = r + v;
	return r;
}
inline Rect& operator-=(Rect& r, const Vector2D& v)
{
	r = r - v;
	return r;
}

#endif
