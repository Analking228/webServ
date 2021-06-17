#include "ListeningSocket.hpp"

HDE::ListeningSocket::ListeningSocket(int domain, int service,
			int protocol, int port, u_long interface, int backlog) :
			BindingSocket(domain, service, protocol, port, interface) {

	_Backlog = backlog;
	startListening();
	testConnection(_Listening);
}

void	HDE::ListeningSocket::startListening() {

	_Listening = listen(getSock(), _Backlog);
}