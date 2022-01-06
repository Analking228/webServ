/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpSpecialResponse.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 20:04:46 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/19 21:21:30 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SPECIAL_RESPONSE_HPP
# define HTTP_SPECIAL_RESPONSE_HPP

namespace HDE
{
	class HttpSpecialResponse
	{
		private:
				std::string _notFound;
				std::string	_notFoundFile;
		public:
				HttpSpecialResponse();
				HttpSpecialResponse(std::string notFoundFile);
				~HttpSpecialResponse();
				std::string getNotFound();
				void 		makeNotFound(std::string notFoundFile);
	};
};

# endif
