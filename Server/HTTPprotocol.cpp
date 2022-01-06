/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPprotocol.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 22:20:45 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/23 22:20:45 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <cstring>


#include "HTTPprotocol.hpp"
#include "Directory.hpp"

#define CRLF "\r\n"

HDE::HTTPprotocol::HTTPprotocol(int socket, std::string workdir,
		std::string url, std::string nf404, std::string del, int ercode) : _Socket(socket), _WorkDir(workdir),
		_Url(url), _404(nf404), _Del(del), _ErrorCode(ercode)
{	
	std::ifstream *file = new std::ifstream(_Url);
	std::cout << _Del << std::endl;
	if (!(file->is_open()) && _Del != "del" && _Del !="large" && _Del != "notallowed")
	{
		file->close();
		_Url = _404;
		_ErrorCode = 404;
		delete file;
		std::ifstream *file = new std::ifstream(_Url);
	}

	std::cout << "url: " << _Url << std::endl;

	if (_Url.find("403") != std::string::npos)
		_ErrorCode = 403;

	contentLenght(file);
	makeHead();
	writeToSocket(file);

	file->close();
	delete file;
}

void		HDE::HTTPprotocol::makeHead()
{
	std::time_t		seconds = time(NULL);
	std::tm*		timeinfo = localtime(&seconds);
	std::string		stime = asctime(timeinfo);

	stime.erase(stime.find('\n'), 1);

	this->_Header = "HTTP/1.1 ";
	this->_Header.append(std::to_string(_ErrorCode)).append(stringStatus(_ErrorCode)).append(CRLF);
	this->_Header.append("Date: ").append(stime).append(" GMT").append(CRLF);
	this->_Header.append("Server: OURS").append(CRLF);
	this->_Header.append("Accept-Ranges: ").append("bytes").append(CRLF);
	this->_Header.append("cache-control: no-cache").append(CRLF);
	this->_Header.append("Content-Length: ").append(std::to_string(_ContentLength)).append(CRLF);
	this->_Header.append("Content-Type: ").append(contentType()).append(CRLF).append(CRLF);
}

void		HDE::HTTPprotocol::contentLenght(std::ifstream* f)
{
	if (_Del == "del")
		_ContentLength = 0;
	else
	{
		f->seekg(0, std::ios::end);
		_ContentLength = f->tellg();
		f->seekg(0, std::ios::beg);
	}
}

void	HDE::HTTPprotocol::writeToSocket(std::ifstream* file)
{
	std::string		links;

	char	*str = new char[_ContentLength];

	file->read(str, _ContentLength);
	size_t w1 = write (_Socket, this->_Header.c_str(), strlen(this->_Header.c_str()));
	if (w1 < 0)
	{
		std::cout << "sorry, I can`t write" << std::endl;
		exit (-1);
	}

	size_t w2 = write (_Socket, str, _ContentLength);
	if (w2 < 0)
	{
		std::cout << "sorry, I can`t write" << std::endl;
		exit (-1);
	}
	delete[] str;
}

std::string		HDE::HTTPprotocol::contentType()
{
	std::string	types[9] = {".html", ".css", ".xml", ".gif", ".jpeg",
		".jpg", ".ico", ".js", ".pdf"};
	
	std::string	ctypes[9] = {"text/html", "text/css", "text/xml",
		"image/gif", "image/jpeg", "image/jpg", "image/x-icon", "application/javascript",
		"application/pdf"};

	for (int i = 0; i < 9; i++)
	{
		int n = _Url.find(types[i]);
		if (n!= std::string::npos)
			return (std::string(ctypes[i]));	
	}
	return (std::string("text/html"));
}

std::string		HDE::HTTPprotocol::stringStatus(int statuscode)
{
	int			arstatus[15] = {100, 200, 204, 206, 300, 301, 302, 400, 401, 403, 404, 408, 411, 415, 500};
	
	std::string	arstring[15] = {" Continue", " OK", " No Content", " Partial Content",
		" Multiple Choices", " Moved Permament", " Moved Temporarily",
		" Bad Request", " Unathorized", " Forbidden", " Not Found", " Request Timeout"
		" Length Required", " Unsupported Media Type", " Internal Server Error"};
	
	for (int i = 0; i < 15; i++)
		if (arstatus[i] == statuscode)
			return (std::string(arstring[i]));
	return (std::string("Unknown status code"));
}
