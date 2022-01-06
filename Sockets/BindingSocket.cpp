#include "BindingSocket.hpp"

HDE::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, 
	std::string interface) : ASimpleSocket(domain, service, protocol, port, interface) 
{
	setConnection(connectNetwork(getSock(), getAddress()));
	testConnection(getConnection());
};

//Defenition of connection
int HDE::BindingSocket::connectNetwork(int sock, struct sockaddr_in address)
{
	int yes = 1;
	int b ;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))
		{
			std::cout << "Sorry, but you must Set option of a socket" << std::endl;
			exit(0);
		}
	b = bind(sock, (struct sockaddr *)&address, sizeof(address));
	std::cout << b << " THIS IS BIND" << std::endl;
	// if (b < 0)
	// {
	// 	std::cout << "Sorry, but I can`t bind" << std::endl;
	// 	exit(0);
	// };
	return (b);
}
