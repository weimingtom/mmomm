#ifndef USER_H_
#define USER_H_

#include <string>
#include <RakNet/RakNetTypes.h>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "collision.h"
#include "vector2D.h"

class Actor;


class User {
public:
	User(const std::string& username, const SystemAddress& address)
		: _username(username), _address(address) { }
	
	// Constant data
	std::string username() const { return _username; }
	SystemAddress address() const { return _address; }
	
	// Send actor updates centered on the given position
	// Network-intensive; do not call every frame.
	void sendNetworkUpdate(const Actor *userActor);

	// Send world updates centered on the given position
	// Not-so-network-intensive; can call every frame.
	void sendWorldMapUpdate(const Actor *userActor);

private:
	// Data we last told the user about
	struct ActorData {
		ActorID id;
		Vector2D position;
		Vector2D velocity;
		double time;
	};

	// What actors does this user know about; what'd we tell the player?
	typedef boost::unordered_map< Actor *, ActorData > PacketMap;
	PacketMap _packetMap;

	typedef User::PacketMap::value_type PacketValue;
	// Get a score on the specified data
	struct ActorScore;
	struct ActorScoreComparator;
	static double score(const PacketValue& value, const Vector2D& reference);
	
	// What cells this actor has been told about, and when
	typedef boost::unordered_map< IVector2D, double > CellMap;
	CellMap _cell;
	
	std::string _username;
	SystemAddress _address;
};

#endif
