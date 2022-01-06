/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASimpleServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 22:38:26 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/12 18:49:56 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASimpleServer.hpp"

HDE::ASimpleServer::ASimpleServer(int domain, int service, int protocol, int port,
		std::string interface, int backlog)
{
	std::cout << "Server stat:\n" << "Domain: " << domain << "\n"
	<< "Service: " << service << "\n"
	<< "Protocol: " << protocol << "\n"
	<< "Port: " << port << "\n"
	<< "Interface: " << interface << "\n"
	<< "Backlog: " << backlog << std::endl;
	_Socket = new ListeningSocket(domain, service, protocol,
		port, interface, backlog);
}

HDE::ListeningSocket * HDE::ASimpleServer::getSocket()
{
	return _Socket;
}
