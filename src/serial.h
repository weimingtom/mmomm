#ifndef SERIAL_H_
#define SERIAL_H_

#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include <RakNet/NativeTypes.h>
#include <boost/static_assert.hpp>
#include "rect.h"
#include "vector2D.h"

using RakNet::BitStream;

BOOST_STATIC_ASSERT(sizeof(uint8_t) == 1);
BOOST_STATIC_ASSERT(sizeof(int8_t) == 1);
BOOST_STATIC_ASSERT(sizeof(uint16_t) == 2);
BOOST_STATIC_ASSERT(sizeof(int16_t) == 2);
BOOST_STATIC_ASSERT(sizeof(uint32_t) == 4);
BOOST_STATIC_ASSERT(sizeof(int32_t) == 4);
BOOST_STATIC_ASSERT(sizeof(float) == 4);
BOOST_STATIC_ASSERT(sizeof(double) == 8);

// Boolean compression
inline void serial(BitStream& bs, bool write, bool& data)
{
	bs.SerializeCompressed(write, data);
}

// Normal integer serialization; better if use less than half
inline void serial(BitStream& bs, bool write, uint8_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, int8_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, uint16_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, int16_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, uint32_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, int32_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, uint64_t& data)
{
	bs.SerializeCompressed(write, data);
}
inline void serial(BitStream& bs, bool write, int64_t& data)
{
	bs.SerializeCompressed(write, data);
}

// Full serialization; no difference no matter how many bits you use
inline void serialFull(BitStream& bs, bool write, uint8_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, int8_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, uint16_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, int16_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, uint32_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, int32_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, uint64_t& data)
{
	bs.Serialize(write, data);
}
inline void serialFull(BitStream& bs, bool write, int64_t& data)
{
	bs.Serialize(write, data);
}

// Full-quality float/double/vector/rect serialization
inline void serial(BitStream& bs, bool write, double& data)
{
	bs.Serialize(write, data);
}
inline void serial(BitStream& bs, bool write, float& data)
{
	bs.Serialize(write, data);
}
inline void serial(BitStream& bs, bool write, Vector2D& data)
{
	serial(bs, write, data.x);
	serial(bs, write, data.y);
}
inline void serial(BitStream& bs, bool write, Rect& data)
{
	serial(bs, write, data.left);
	serial(bs, write, data.top);
	serial(bs, write, data.right);
	serial(bs, write, data.bottom);
}

// Serialize using lossy compression to single floating point precision.
void serialSingle(BitStream& bs, bool write, double& data);
inline void serialSingle(BitStream& bs, bool write, float& data)
{
	serial(bs, write, data);
}
inline void serialSingle(BitStream& bs, bool write, Vector2D& data)
{
	serialSingle(bs, write, data.x);
	serialSingle(bs, write, data.y);
}
inline void serialSingle(BitStream& bs, bool write, Rect& data)
{
	serialSingle(bs, write, data.left);
	serialSingle(bs, write, data.top);
	serialSingle(bs, write, data.right);
	serialSingle(bs, write, data.bottom);
}

// Serialize using lossy compression to half floating point precision.
// Limit is fixed to +-65504. 5-bit exponent, 11-bit mantissa.
void serialHalf(BitStream& bs, bool write, double& data);
void serialHalf(BitStream& bs, bool write, float& data);
inline void serialHalf(BitStream& bs, bool write, Vector2D& data)
{
	serialHalf(bs, write, data.x);
	serialHalf(bs, write, data.y);
}
inline void serialHalf(BitStream& bs, bool write, Rect& data)
{
	serialHalf(bs, write, data.left);
	serialHalf(bs, write, data.top);
	serialHalf(bs, write, data.right);
	serialHalf(bs, write, data.bottom);
}

// Serialize a displacement from a given position.
// Limit is fixed to +-64 coords away.
void serialDisplacement(BitStream& bs, bool write, double& data);
void serialDisplacement(BitStream& bs, bool write, float& data);
inline void serialDisplacement(BitStream& bs, bool write, Vector2D& data)
{
	serialDisplacement(bs, write, data.x);
	serialDisplacement(bs, write, data.y);
}
inline void serialDisplacement(BitStream& bs, bool write, Rect& data)
{
	serialDisplacement(bs, write, data.left);
	serialDisplacement(bs, write, data.top);
	serialDisplacement(bs, write, data.right);
	serialDisplacement(bs, write, data.bottom);
}

// Serialize an absolute position.
// Limit is fixed to +-2^22 (over 2 million) coords away.
void serialPosition(BitStream& bs, bool write, double& data);
inline void serialPosition(BitStream& bs, bool write, float& data)
{
	// Floats are already 16 bits, so just use them as normal
	serial(bs, write, data);
}
inline void serialPosition(BitStream& bs, bool write, Vector2D& data)
{
	serialPosition(bs, write, data.x);
	serialPosition(bs, write, data.y);
}
inline void serialPosition(BitStream& bs, bool write, Rect& data)
{
	serialPosition(bs, write, data.left);
	serialPosition(bs, write, data.top);
	serialPosition(bs, write, data.right);
	serialPosition(bs, write, data.bottom);
}

// Serialize a velocity.
// Half single precision; Limit is fixed to +-65504 coords per second.
inline void serialVelocity(BitStream& bs, bool write, double& data)
{
	serialHalf(bs, write, data);
}
inline void serialVelocity(BitStream& bs, bool write, float& data)
{
	serialHalf(bs, write, data);
}
inline void serialVelocity(BitStream& bs, bool write, Vector2D& data)
{
	serialHalf(bs, write, data);
}

// Serialize a whole std::vector.
// Limited to 65535 entries.
template<typename T>
void serial(BitStream& bs, bool write, std::vector<T>& data)
{
	if (write) {
		uint16_t size = data.size();
		assert(size < (2 << 16));
		serial(bs, write, size);
		BOOST_FOREACH(T& value, data) {
			serial(bs, write, value);
		}
	}
	else {
		uint16_t size;
		serial(bs, write, size);
		data.resize(size);
		for (unsigned i = 0; i < size; ++i) {
			serial(bs, write, data[i]);
		}
	}
}

// Serialize a string; compresses the string.
void serial(BitStream& bs, bool write, std::string& data);

#endif