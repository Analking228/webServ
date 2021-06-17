#include "ConnectingSocket.hpp"

HDE::ConnectingSocket::ConnectingSocket(int domain, int service,
		int protocol, int port, u_long interface) :
		ASimpleSocket(domain, service, protocol, port,
		interface) {

	setConnection(connectNetwork(getSock(), getAddress()));
	testConnection(getConnection());
}

int		HDE::ConnectingSocket::connectNetwork(int sock, struct
		sockaddr_in address) {

	return connect(sock, (struct sockaddr *)&address, sizeof(address));
}