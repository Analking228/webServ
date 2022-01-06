/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpSpecialResponse.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 20:04:41 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/20 12:31:09 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <cstring>

#include "HttpSpecialResponse.hpp"

#define CRLF "\r\n"


HDE::HttpSpecialResponse::HttpSpecialResponse()
{
	this->makeNotFound("");
}

HDE::HttpSpecialResponse::HttpSpecialResponse(std::string notFoundFile) : _notFoundFile(notFoundFile)
{
	this->makeNotFound(notFoundFile);
}

HDE::HttpSpecialResponse::~HttpSpecialResponse()
{
	
}

std::string HDE::HttpSpecialResponse::getNotFound()
{
	return this->_notFound;
}

void HDE::HttpSpecialResponse::makeNotFound(std::string notFoundFile)
{
	std::ifstream 	ifs;
	int				n = 256;
	
	this->_notFound = "";
	ifs.open(notFoundFile);
	if (!strcmp(notFoundFile.c_str(), "") || !ifs)
			this->_notFound = 
				"<html>" CRLF
				"<head><title>TEST 404 Not Found</title></head>" CRLF
				"<body>" CRLF
				"<center><h1>404 Not Found</h1></center>" CRLF
				"</body>" CRLF
				"</html>";
	else
	{
		char *buffer = new char[n + 1];
		while (!ifs.eof())
		{
			buffer[n] = 0;
			ifs.getline(buffer, n, '\n');
			this->_notFound = this->_notFound.append(std::string(buffer)).append(CRLF);
		}
		delete [] buffer;
	}
	ifs.close();
}
