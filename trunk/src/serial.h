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

//////////////////////////////////////////////////////////////////////
//
// Integer serialization
// 
// serial:     General lossless compressing serialization.
//             Use this if you usually use less than half the bits.
//
// serialFull: No compression serialization
//             Use this if you usually use the full range of values.
//
//////////////////////////////////////////////////////////////////////

// Normal integer serialization; better if usually use less than half
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

//////////////////////////////////////////////////////////////////////
//
// Floating point serialization
// 
// serial:       General lossless serialization (64 bits).
//               Use this if you need full quality.
//               subnormal min: 5.00e-324
//               normal min:    2.23e-308
//               max:           1.80e+308
//               accuracy:      15.955 decimal places
//
// serialSingle: Compress to single-precision floating point (32 bits).
//               subnormal min: 1.40e-45
//               normal min:    1.18e-38
//               max:           3.40e+38
//               accuracy:      7.225 decimal places
//
// serialHalf:   Compress to half-precision floating point (16 bits).
//               subnormal min: 5.96e-8
//               normal min:    6.10e-5
//               max:           65504
//               accuracy:      3.311 decimal places
//
//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////
//
// Serialization of position, displacement and velocity
// 
// serialPosition:     Limited to (-32768, 32768).
//                     Bits: 16 (integer), 32 (float)
//                     Use for absolute positioning on the world map.
// 
// serialDisplacement: Limited to (-64, 64). 
//                     Bits: 8 (integer), 16 (float).
//                     Use for positioning relative to a known point.
// 
// serialVelocity:     Biased; stored as half-precision float.
//                     Bits: 16 (float only)
//                     Use for velocities.
//
//////////////////////////////////////////////////////////////////////

// Serialize an absolute position.
void serialPosition(BitStream& bs, bool write, int32_t& data);
inline void serialPosition(BitStream& bs, bool write, IVector2D& data)
{
	serialPosition(bs, write, data.x);
	serialPosition(bs, write, data.y);
}
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

// Serialize a displacement from a known point.
void serialDisplacement(BitStream& bs, bool write, int32_t& data);
inline void serialDisplacement(BitStream& bs, bool write, IVector2D& data)
{
	serialDisplacement(bs, write, data.x);
	serialDisplacement(bs, write, data.y);
}
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

// Serialize a velocity.
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

//////////////////////////////////////////////////////////////////////
//
// Miscellaneous Compression
// 
// boolean:   Serialized as one bit
//
// vector<T>: Serialized as 16-bit length + each entry
//            Limited to 65535 entries
//
// string:    Serialized with huffman compression.
//
//////////////////////////////////////////////////////////////////////

// Boolean compression
inline void serial(BitStream& bs, bool write, bool& data)
{
	bs.SerializeCompressed(write, data);
}

// Serialize a whole std::vector.
// Limited to 65535 entries.
template<typename T>
void serial(BitStream& bs, bool write, std::vector<T>& data)
{
	if (write) {
		assert(data.size() < 2 << 16);
		uint16_t size = uint16_t(data.size());
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