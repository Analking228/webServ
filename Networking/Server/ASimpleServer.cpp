#include "ASimpleServer.hpp"

HDE::ASimpleServer::ASimpleServer(int domain, int service, int protocol,
		int port, u_long interface, int backlog) {

	_Socket = new ListeningSocket(domain, service, protocol, port,
		interface, backlog);
	
}

HDE::ListeningSocket*	HDE::ASimpleServer::getSocket() {

	return _Socket;
}