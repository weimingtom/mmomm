#ifndef SERIALIZATION_EXCEPTION_H_
#define SERIALIZATION_EXCEPTION_H_

// Custom exceptions related to serialization.

#include <stdexcept>
#include <sstream>

// An exception which indicates an error loading or storing data.
struct SerializationException: public std::runtime_error {
	SerializationException(const std::string& message)
		: std::runtime_error(message) { }
};

// Macro for easy throwing.
#define SERIALIZATION_EXCEPTION(msg) \
	do { \
		std::stringstream _p_SS; \
		_p_SS << msg; \
		throw new SerializationException(_p_SS.str()); \
	} while (false)

#endif
