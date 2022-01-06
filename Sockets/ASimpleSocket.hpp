/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:38:25 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/10 22:18:50 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

namespace HDE
{

	class ASimpleSocket
	{
	private:
		struct sockaddr_in 	_Address;
		int 				_Sock;
		int 				_Connection;

	public:
		ASimpleSocket(int domain, int service, int protocol, int port,
			std::string interface);
	
		struct sockaddr_in	getAddress();
		int					getSock();
		int					getConnection();
	
		void				setConnection(int connection);

		virtual int			connectNetwork(int sock, struct sockaddr_in
			address) = 0;
		void				testConnection(int);	
	};
}
