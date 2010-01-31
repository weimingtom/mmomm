#ifndef USER_H_
#define USER_H_

#include <string>
#include <RakNet/RakNetTypes.h>

class User {
public:
	User(const std::string& username, const SystemAddress& address)
		: _username(username), _address(address) { }
	
	// Constant data
	std::string username() const { return _username; }
	SystemAddress address() const { return _address; }

private:
	std::string _username;
	SystemAddress _address;
};

#endif