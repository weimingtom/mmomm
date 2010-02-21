#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <cmath>
#include <cfloat>
#include <RakNet/NativeTypes.h>
#include <boost/functional/hash.hpp>

// A mostly-immutable 2-dimensional vector
template<typename T>
struct GVector2D {
	GVector2D(): x(), y() { }
	template<typename U>
	GVector2D(U x, U y): x(T(x)), y(T(y)) { }
	template<typename U>
	explicit GVector2D(const GVector2D<U>& rhs): x(T(rhs.x)), y(T(rhs.y)) { }
	
	// Returns the length squared of this vector (faster than length()).
	T lengthSquared() const;
	// Returns the length of this vector.
	T length() const;
	// Returns a vector pointing in the same direction, of length 1.
	GVector2D normalized() const;
	
	// Memberwise multiplication/division
	GVector2D memberwiseMult(const GVector2D& rhs) const;
	GVector2D memberwiseDiv(const GVector2D& rhs) const;
	GVector2D memberwiseMod(const GVector2D& rhs) const;
	
	// Returns the dot product of this and another vector.
	// |a||b|cos(theta)
	T dot(const GVector2D& rhs) const;
	// Returns the perpendicular dot product of this and another vector.
	// Similar to cross product, or wedge product.
	// |a||b|sin(theta)
	T perpDot(const GVector2D& rhs) const;
	
	// x and y coordinates
	T x;
	T y;

	// Returns a hash of the given value.
    friend std::size_t hash_value(const GVector2D& v)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, v.x);
        boost::hash_combine(seed, v.y);
        return seed;
    }
};

// Negation
template<typename T>
inline GVector2D<T> operator+(const GVector2D<T>& v)
{
	return v;
}
template<typename T>
inline GVector2D<T> operator-(const GVector2D<T>& v)
{
	return GVector2D<T>(-v.x, -v.y);
}

// Vector addition/subtraction
template<typename T>
inline GVector2D<T> operator+(const GVector2D<T>& v1, const GVector2D<T>& v2)
{
	return GVector2D<T>(v1.x + v2.x, v1.y + v2.y);
}
template<typename T>
inline GVector2D<T> operator-(const GVector2D<T>& v1, const GVector2D<T>& v2)
{
	return GVector2D<T>(v1.x - v2.x, v1.y - v2.y);
}
template<typename T>
inline GVector2D<T>& operator+=(GVector2D<T>& v1, const GVector2D<T>& v2)
{
	return v1 = v1 + v2;
}
template<typename T>
inline GVector2D<T>& operator-=(GVector2D<T>& v1, const GVector2D<T>& v2)
{
	return v1 = v1 - v2;
}

// Scalar multiplication/division
template<typename T>
inline GVector2D<T> operator*(const GVector2D<T>& v, T scalar)
{
	return GVector2D<T>(v.x * scalar, v.y * scalar);
}
template<typename T>
inline GVector2D<T> operator*(T scalar, const GVector2D<T>& v)
{
	return v * scalar;
}
template<typename T>
inline GVector2D<T> operator/(const GVector2D<T>& v, T scalar)
{
	return GVector2D<T>(v.x / scalar, v.y / scalar);
}
template<typename T>
inline GVector2D<T> operator/(T scalar, const GVector2D<T>& v)
{
	return GVector2D<T>(scalar / v.x, scalar / v.y);
}
template<typename T>
inline GVector2D<T> operator%(const GVector2D<T>& v, T scalar)
{
	return GVector2D<T>(v.x % scalar, v.y % scalar);
}

template<typename T>
inline GVector2D<T>& operator*=(GVector2D<T>& v, T scalar)
{
	return v = v * scalar;
}
template<typename T>
inline GVector2D<T>& operator/=(GVector2D<T>& v, T scalar)
{
	return v = v / scalar;
}
template<typename T>
inline GVector2D<T>& operator%=(GVector2D<T>& v, T scalar)
{
	return v = v % scalar;
}


template<typename T>
inline bool operator==(const GVector2D<T>& v1, const GVector2D<T>& v2)
{
	return (v1.x == v2.x && v1.y == v2.y);
}
template<typename T>
inline bool operator!=(const GVector2D<T>& v1, const GVector2D<T>& v2)
{
	return !(v1 == v2);
}

template<typename T>
inline GVector2D<T> GVector2D<T>::memberwiseMult(const GVector2D<T>& rhs) const
{
	return GVector2D<T>(x * rhs.x, y * rhs.y);
}
template<typename T>
inline GVector2D<T> GVector2D<T>::memberwiseDiv(const GVector2D<T>& rhs) const
{
	return GVector2D<T>(x / rhs.x, y / rhs.y);
}
template<typename T>
inline GVector2D<T> GVector2D<T>::memberwiseMod(const GVector2D<T>& rhs) const
{
	return GVector2D<T>(x % rhs.x, y % rhs.y);
}

template<typename T>
inline T GVector2D<T>::lengthSquared() const
{
	return this->dot(*this);
}
template<typename T>
inline T GVector2D<T>::length() const
{
	return std::sqrt(lengthSquared());
}
template<typename T>
inline GVector2D<T> GVector2D<T>::normalized() const
{
    return lengthSquared() >= FLT_EPSILON * FLT_EPSILON ? *this / length() : GVector2D<T>();
}

template<typename T>
inline T GVector2D<T>::dot(const GVector2D<T>& rhs) const
{
	return x * rhs.x + y * rhs.y;
}
template<typename T>
inline T GVector2D<T>::perpDot(const GVector2D<T>& rhs) const
{
	return x * rhs.y - y * rhs.x;
}

// Not working for some reason.
/*
namespace std {

// Returns the minimal/maximal coordinates of the pair.
inline ::GVector2D min(const ::GVector2D& v1, const ::GVector2D& v2)
{
	return ::Vector2D(min(v1.x, v2.x), min(v1.y, v2.y));
}
inline ::GVector2D max(const ::GVector2D& v1, const ::GVector2D& v2)
{
	return ::Vector2D(max(v1.x, v2.x), max(v1.y, v2.y));
}

}
*/

typedef GVector2D<double> Vector2D;
typedef GVector2D<int32_t> IVector2D;

#endif
