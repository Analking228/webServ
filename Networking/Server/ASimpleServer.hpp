#pragma once

#include <unistd.h>
#include <string.h>

#include "../hdelibc-networking.hpp"

namespace HDE {

class ASimpleServer {

private:
	ListeningSocket*	_Socket;
	virtual void		accepter() = 0;
	virtual void		handler() = 0;
	virtual void		responder() = 0;

public:
	ASimpleServer(int domain, int service, int protocol,
		int port, u_long interface, int backlog);

	virtual void		launch() = 0;

	ListeningSocket*	getSocket();
};
}