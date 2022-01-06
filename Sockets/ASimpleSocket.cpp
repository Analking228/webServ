/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASimpleSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:38:28 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/10 22:18:46 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "ASimpleSocket.hpp"

HDE::ASimpleSocket::ASimpleSocket(int domain, int service, int protocol, int port,
	std::string interface)
{
	// define address structure
	if (interface == "localhost")
		interface = "127.0.0.1";
	_Address.sin_family = domain;
	_Address.sin_port = htons(port);
	_Address.sin_addr.s_addr = inet_addr(interface.c_str());
	if (_Address.sin_addr.s_addr == -1)
	{
		perror ("Failured to interpretate address");
		exit(EXIT_FAILURE);
	}
	// establish socket
	_Sock = socket(domain, service, protocol);
	testConnection(_Sock);
}

void HDE::ASimpleSocket::testConnection(int item_to_test)
{
	if (item_to_test < 0)
	{
		perror ("Failured to connect...");
		exit(EXIT_FAILURE);
	}
	
}

struct sockaddr_in HDE::ASimpleSocket::getAddress()
{
	return this->_Address;
}

int HDE::ASimpleSocket::getSock()
{
	return this->_Sock;
}

int HDE::ASimpleSocket::getConnection()
{
	return this->_Connection;
}

void HDE::ASimpleSocket::setConnection(int connection)
{
	this->_Connection = connection;
}
