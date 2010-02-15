#ifndef SERIALIZATION_EXCEPTION_H_
#define SERIALIZATION_EXCEPTION_H_

// Custom exceptions related to serialization.

#include <stdexcept>

// An exception which indicates an error loading the data.
struct LoadException: public std::runtime_error {
	LoadException(const std::string& message)
		: std::runtime_error(message) { }
};

#endif
