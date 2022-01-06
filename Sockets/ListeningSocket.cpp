/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 21:34:10 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/10 22:32:41 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

HDE::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, 
	std::string interface, int backlog) : BindingSocket(domain, service, protocol,
	port, interface), _Backlog(backlog)
{
	startListening();
	testConnection(_Listening);
};

void HDE::ListeningSocket::startListening()
{
	_Listening = listen(getSock(), _Backlog);
}
