/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 22:38:24 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/12 18:53:09 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../hdelibc.hpp"

namespace HDE
{
class ASimpleServer
{
private:
		ListeningSocket*	_Socket;
		
		virtual int			acceptor() = 0;
		virtual void		handler() = 0;
		virtual void		responder(int newSocket) = 0;

public:
		ASimpleServer(int domain, int service, int protocol, int port,
			std::string interface, int backlog);
		
		virtual void		launch() = 0;
		
		ListeningSocket*	getSocket();			
};
}
