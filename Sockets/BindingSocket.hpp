/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BindingSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:31:31 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/10 20:55:39 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ASimpleSocket.hpp"

namespace HDE
{
	class BindingSocket : public ASimpleSocket
	{
	public:
		BindingSocket(int domain, int service, int protocol, int port, 
			std::string interface);
	
		virtual int connectNetwork(int sock, struct sockaddr_in address);
	};
};
