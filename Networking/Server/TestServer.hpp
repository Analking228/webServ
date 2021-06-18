#pragma once

#include "ASimpleServer.hpp"

namespace HDE {

class TestServer : public ASimpleServer {

private:
	char		_Buffer[30000] = {0};
	int			_NewSocket;

	void		accepter();
	void		handler();
	void		responder();

public:
	TestServer();

	void		launch();
};
}