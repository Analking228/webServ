#pragma once

#include "ASimpleSocket.hpp"

namespace HDE {

class ConnectingSocket : public ASimpleSocket {

public:
	ConnectingSocket(int domain, int service, int protocol,
		int port, u_long interface);

	int		connectNetwork(int sock, struct sockaddr_in address);
};
}