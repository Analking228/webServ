#include "BindingSocket.hpp"

HDE::BindingSocket::BindingSocket(int domain, int service,
		int protocol, int port, u_long interface) :
		ASimpleSocket(domain, service, protocol, port,
		interface) {
			
	setConnection(connectNetwork(getSock(), getAddress()));
	testConnection(getConnection());
}

//Defenition of connection
int		HDE::BindingSocket::connectNetwork(int sock, struct
		sockaddr_in address) {

	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}