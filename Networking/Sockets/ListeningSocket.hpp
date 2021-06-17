#pragma once

#include "BindingSocket.hpp"

namespace HDE {

class ListeningSocket : public BindingSocket {

private:
	int		_Backlog;
	int		_Listening;

public:
	ListeningSocket(int domain, int service, int protocol,
		int port, u_long interface, int backlog);

	void	startListening();
	
};
}