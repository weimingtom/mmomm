#include "networkClient.h"

NetworkClient::NetworkClient()
:	_peer()
{
}

void NetworkClient::connect(const std::string& host, int port)
{
    assert(!isConnected());
    assert(port >= 0 && port <= 65535);
    
	RakPeerInterface* peer = RakNetworkFactory::GetRakPeerInterface();

	int threadSleepMillis = 30;
	SocketDescriptor descriptor(0, 0)
	if (peer->Startup(1, threadSleepMillis, &descriptor, 1)) {
		
		unsigned short shrunkenPort = static_cast<unsigned short>(port);
		if (peer->Connect(host.c_str(), shrunkenPort, 0, 0)) {
			peer->SetOccasionalPing(true);
			_peer = peer;
			return true;
		}
		
		// Probably nothing to send; don't need to wait very long.
		peer->Shutdown(100);
	}
	RakNetworkFactory::DestroyRakPeerInterface(peer);
	return false;
}

void NetworkClient::disconnect()
{
    if (isConnected()) {
		// Wait for 1 second to try to get everything out
        _peer->Shutdown(1000);
        RakNetworkFactory::DestroyRakPeerInterface(_peer);
        _peer = 0;
    }
}
