#include "ASimpleSocket.hpp"

//Default constructor

HDE::ASimpleSocket::ASimpleSocket(int domain, int service, int protocol, int port,
	u_long interface) {

	//Define address structure
	_Address.sin_family = domain;
	_Address.sin_port = htons(port);
	_Address.sin_addr.s_addr = htonl(interface);
	//Establish socket
	_Sock = socket(domain, service, protocol);
	testConnection(_Sock);
	//Establish connection | it should be added in child classes separately in constructor
	//_Connection = connectNetwork(_Sock, _Address);
	testConnection(_Connection);
}

//Test connection virtual func

void	HDE::ASimpleSocket::testConnection(int testItem) {

	//Check if the connection has been est
	if (testItem < 0) {
		perror("Failed to connect...");
		exit(EXIT_FAILURE);
	}
}

//Getters

struct sockaddr_in HDE::ASimpleSocket::getAddress() {
	return _Address;
}

int	HDE::ASimpleSocket::getSock() {
	return _Sock;
}

int	HDE::ASimpleSocket::getConnection() {
	return _Connection;
}

void	HDE::ASimpleSocket::setConnection(int connection) {
	_Connection = connection;
}