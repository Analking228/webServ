#pragma once

#include "ASimpleSocket.hpp"

namespace HDE {
	
class BindingSocket: public ASimpleSocket {

public:
	BindingSocket(int domain, int service, int protocol,
		int port, u_long interface);
		
	int		connectNetwork(int sock, struct sockaddr_in address);
};
}
