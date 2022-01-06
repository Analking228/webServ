/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 21:34:08 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/10 21:50:16 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "BindingSocket.hpp"

namespace HDE
{
	class ListeningSocket : public BindingSocket
	{
	private:
		int 	_Backlog;
		int 	_Listening;
	public:
		ListeningSocket(int domain, int service, int protocol, int port, 
			std::string interface, int backlog);
		
		void startListening();
				
	};
}
