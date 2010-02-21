#ifndef SERIALIZATION_EXCEPTION_H_
#define SERIALIZATION_EXCEPTION_H_

// Custom exceptions related to serialization.

#include <stdexcept>
#include <sstream>

// An exception which indicates an error loading or storing data.
struct InvalidPacketException: public std::runtime_error {
	InvalidPacketException(const std::string& message)
		: std::runtime_error(message) { }
};

// An exception which indicates an error loading or storing data.
struct SerializationException: public std::runtime_error {
	SerializationException(const std::string& message)
		: std::runtime_error(message) { }
};

// Macros for easy throwing.
#define THROW_EXCEPTION(type, msg) \
	do { \
		std::stringstream _p_SS; \
		_p_SS << msg; \
		throw new type(_p_SS.str()); \
	} while (false)

#define INVALID_PACKET_EXCEPTION(msg) THROW_EXCEPTION(InvalidPacketException, msg)
#define SERIALIZATION_EXCEPTION(msg) THROW_EXCEPTION(SerializationException, msg)

#endif
