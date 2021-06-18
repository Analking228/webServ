#include "TestServer.hpp"

HDE::TestServer::TestServer() : ASimpleServer(AF_INET, SOCK_STREAM,
		0, 80, INADDR_ANY, 10) {
	
	launch();
}

void	HDE::TestServer::accepter() {

	struct sockaddr_in	address = getSocket()->getAddress();
	int					addrlen = sizeof(address);

	_NewSocket = accept(getSocket()->getSock(), (struct sockaddr*)
		&address, (socklen_t*)&addrlen);
	//Заменить на нормальное вычисление длинны
	read(_NewSocket, _Buffer, 30000);
}

void	HDE::TestServer::handler() {

	std::cout << _Buffer << std::endl;
}

void	HDE::TestServer::responder() {

	char*	hello = "Hello from Server";

	write(_NewSocket, hello, strlen(hello));
	close(_NewSocket);
}

void	HDE::TestServer::launch() {

	while(1 < 2) {

		std::cout << "====== WAITTING ======" << std::endl;
		accepter();
		handler();
		responder();
		std::cout << "===== DONE =====" << std::endl;
	}
}