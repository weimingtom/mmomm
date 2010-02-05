#include "serial.h"
#include "ieeehalfprecision.h"
#include <RakNet/StringCompressor.h>

void serialSingle(BitStream& bs, bool write, double& data)
{
	float single;
	if (write) single = float(data);
	serial(bs, write, single);
	if (!write) data = single;
}

void serialHalf(BitStream& bs, bool write, double& data)
{
	uint16_t half;
	if (write) {
		doubles2halfp(&half, &data, 1);	
	}
	serial(bs, write, half);
	if (!write) {
		halfp2doubles(&data, &half, 1);	
	}
}

void serialHalf(BitStream& bs, bool write, float& data)
{
	uint16_t half;
	if (write) {
		singles2halfp(&half, &data, 1);	
	}
	serial(bs, write, half);
	if (!write) {
		halfp2singles(&data, &half, 1);	
	}
}

void serialDisplacement(BitStream& bs, bool write, double& data)
{
	// Maximum original displacement
	const double MAXIMUM = 1 << 6;
	// Offset from original
	const double OFFSET = 1 << 9;
	const double REVERSE_OFFSET = 1 / OFFSET;
	
	int16_t displacement;
	if (write) {
		assert(data > -MAXIMUM || data < MAXIMUM);
		displacement = int16_t(data * OFFSET);
	}
	serialFull(bs, write, displacement);
	if (!write) {
		data = double(displacement) * REVERSE_OFFSET;
	}
}

void serialDisplacement(BitStream& bs, bool write, float& data)
{
	double dbl = data;
	if (write) dbl = data;
	serialDisplacement(bs, write, dbl);
	if (!write) data = float(dbl);
}

void serialPosition(BitStream& bs, bool write, double& data)
{
	// Maximum original position
	const double MAXIMUM = 1 << (16 + 6);
	// Offset from original
	const double OFFSET = 1 << 9;
	const double REVERSE_OFFSET = 1 / OFFSET;
	
	int32_t position;
	if (write) {
		assert(data > -MAXIMUM || data < MAXIMUM);
		position = int32_t(data * OFFSET);
	}
	serialFull(bs, write, position);
	if (!write) {
		data = double(position) * REVERSE_OFFSET;
	}
}


// Initializes the string compressor.
struct StringCompressorInitializer {
	StringCompressorInitializer()
	{
		StringCompressor::AddReference();
		// NOTE: Could add custom compression table.
	}
};

StringCompressorInitializer stringCompressorInitializer;

void serial(BitStream& bs, bool write, std::string& data)
{
	const int bufferSize = 256;
	if (write) {
		StringCompressor::Instance()->EncodeString(data.c_str(), bufferSize, &bs);
	}
	else {
		char buffer[bufferSize+1];
		StringCompressor::Instance()->DecodeString(buffer, bufferSize, &bs);
		data = buffer;
	}
}


