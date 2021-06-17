#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace HDE {

class ASimpleSocket {

private:
	struct				sockaddr_in _Address;
	int					_Sock;
	int					_Connection;

public:
	ASimpleSocket(int domain, int service, int protocol, int port, u_long interface);
	
	struct sockaddr_in	getAddress();
	int					getSock();
	int					getConnection();
	
	void				setConnection(int connection);

	virtual int			connectNetwork(int sock, struct sockaddr_in address) = 0;
	void				testConnection(int);
};
}
