/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/12 16:39:40 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/22 17:25:58 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <dirent.h>
#include <poll.h>
#include <cctype>
#include <fstream>

#include "../Client/Parser/Configuration.hpp"
#include "TestServer.hpp"
#include "HTTPprotocol.hpp"
#include "Directory.hpp"
#include "../CGI/CgiEnv.hpp"

extern const unsigned int MAX_CLIENT_NUM = 100*1024 + 10;
extern const char ERR_MESSAGE[] = "[ERR]too many clients!!!";

HDE::TestServer::TestServer() : ASimpleServer(AF_INET/*2*/, SOCK_STREAM/*1*/,
		0, 80, "127.0.0.1", 10)
{
	this->_ServerName = "defaultserver";
	this->_404 = "../tmp/404NotFound.html";
	this->_403 = "../tmp/403Forbidden.html";
	this->_413 = "../tmp/413RequestEntityTooLarge.html";
	this->_405 = "../tmp/405MethodNotAllowed.html";
	this->_Buffer = (char*)malloc(sizeof(char) * 30000);
	launch();
}

HDE::TestServer::TestServer(HDE::Configuration *config) : ASimpleServer(AF_INET, SOCK_STREAM,
		0, config->getServerIterator(0).port, config->getServerIterator(0).host, 10), _Config(config), _Locations(config->getLocations(0)) // добавить инт для определения нужных локейшнс
{
	_ServerIndex = 0;
	ejectConfData();
	this->_Buffer = (char*)malloc(sizeof(char) * 30000);
	launch();
}

HDE::TestServer::TestServer(HDE::Configuration *config, int ServIndex) : ASimpleServer(AF_INET, SOCK_STREAM,
		0, config->getServerIterator(ServIndex).port, config->getServerIterator(ServIndex).host, 10),
		_ServerIndex(ServIndex), _Config(config), _Locations(config->getLocations(ServIndex)) // добавить инт для определения нужных локейшнс
{
	ejectConfData();
	this->_Buffer = (char*)malloc(sizeof(char) * 30000);
	launch();
}

HDE::TestServer::~TestServer()
{
	free(_Buffer);
}

void	HDE::TestServer::ejectConfData()
{
	std::map<int, std::string>::iterator	it, it2, it3, it4;

	this->_ServerName = _Config->getServerIterator(_ServerIndex).names[0];
	
	if ((it = _Config->getServerIterator(_ServerIndex).error_pages.find(404)) != _Config->getServerIterator(_ServerIndex).error_pages.end())
		this->_404 = "../tmp/404NotFound.html";
	else
		this->_404 = _Config->getServerIterator(_ServerIndex).error_pages[404];
	
	if ((it2 = _Config->getServerIterator(_ServerIndex).error_pages.find(403)) != _Config->getServerIterator(_ServerIndex).error_pages.end())
		this->_403 = "../tmp/403Forbidden.html";
	else
		this->_403 = _Config->getServerIterator(_ServerIndex).error_pages[403];

	if ((it3 = _Config->getServerIterator(_ServerIndex).error_pages.find(413)) != _Config->getServerIterator(_ServerIndex).error_pages.end())
		this->_413 = "../tmp/413RequestEntityTooLarge.html";
	else
		this->_413 = _Config->getServerIterator(_ServerIndex).error_pages[413];
		
	if ((it4 = _Config->getServerIterator(_ServerIndex).error_pages.find(405)) != _Config->getServerIterator(_ServerIndex).error_pages.end())
		this->_405 = "../tmp/405MethodNotAllowed.html";
	else
		this->_405 = _Config->getServerIterator(_ServerIndex).error_pages[405];
	std::cout << "Server Root: " << _Config->getServerIterator(_ServerIndex).root << std::endl; 
}

std::vector<std::string> HDE::TestServer::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss;
	
	ss.str(s);
	
	std::string item;
	
	while (std::getline(ss, item, delim))
		elems.push_back(item);
	
	return elems;
}

std::vector<std::string> HDE::TestServer::splitSubstr(const std::string &s, std::string delimiter)
{
	size_t						pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string 				token;
	std::vector<std::string>	res;

	while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}
	res.push_back (s.substr (pos_start));
	return res;
}

int HDE::TestServer::acceptor()
{
	struct sockaddr_in	address = getSocket()->getAddress();
	int					addrlen = sizeof(address);
	
	_NewSocket = accept(getSocket()->getSock(), (struct sockaddr *)&address,
		(socklen_t *)&addrlen);
	this->_Cgi = 0;
	return (_NewSocket);
}

void HDE::TestServer::makeParamsMap(std::vector<std::string> params)
{
	if (params.size() > 1)
	{
		std::cout << "params: " << params[1] << std::endl;
		std::vector<std::string> params2 = split(params[1], '&');
		std::vector<std::string>::iterator it = params2.begin();
		while (it != params2.end())
		{
			std::vector<std::string> params3 = split(*it, '=');
			this->_Params[(params3[0])] = params3[1];
			std::cout << params3[0] << " " << params3[1] << std::endl;
			++it;
		}
	}
}

HDE::CgiEnv* HDE::TestServer::createCgiEnv()
{
	// Вытаскиваем параметры в map, если у нас POST запрос
	if (this->_Method == "POST")
	{		
		std::vector<std::string> params = splitSubstr(_Buffer, "\r\n\r\n");
		int ext = _Url.find(".php");
		if (ext!= std::string::npos)
		{
			makeParamsMap(params);
			this->_Cgi = 1;
			CgiEnv* cgiEnv = new CgiEnv("", params[1], this->_Method);
			cgiEnv->setPEnv();
			return (cgiEnv);
		}
			return (NULL);
	}
	// Вытаскиваем параметры из GET запроса, если они есть
	if (this->_Method == "GET")
	{
		int n = _Url.find("?");
		if (n!= std::string::npos)
		{
			this->_Cgi = 1;
			std::vector<std::string> params = split(_Url, '?');
			makeParamsMap(params);
			CgiEnv* cgiEnv = new CgiEnv(params[1], "", this->_Method);
			cgiEnv->setPEnv();
			return (cgiEnv);
		}
	}
	return NULL;
}

void HDE::TestServer::handler()
{
	std::cout << _Buffer << std::endl;
	
	// Делим буфер сначала на строки по \r\n 
	// Затем делим первую строку по пробелу (на методб url и протокол)
	std::vector<std::string>	bufferLines = splitSubstr(_Buffer, "\r\n");
	std::vector<std::string>	firstBufferLine = split(bufferLines[0], ' ');
	
	this->_Url = firstBufferLine[1];
	this->_Method = firstBufferLine[0];

	// Проверочная распечатка, ее потом можно убрать
	std::cout << "Buffer first line: " << bufferLines[0] << std::endl;
	std::cout << "method: " << firstBufferLine[0] << std::endl;
	std::cout << "url: " << firstBufferLine[1] << std::endl;
	std::cout << "protocol: " << firstBufferLine[2] << std::endl;
}

int HDE::TestServer::isValidMethod(std::string method)
{
	int isValidMethod = 0;
	
	std::vector<std::string>::iterator it = _Locations[_Locindex].methods.begin();
	while (it !=  _Locations[_Locindex].methods.end())
    {
		if (*it == method)
			isValidMethod = 1;
        std::cout << *it << " " << std::endl; // выводим значение элемента, на который указывает итератор
        ++it; // и переходим к следующему элементу
    }
	return (isValidMethod);
}

void HDE::TestServer::responder(int newSocket)
{
	// место для проверки локейшнов
	
	std::vector<HDE::Configuration::location>::iterator	it, it2;

	it2 = _Locations.end();
	int i = 0;
	_Locindex = 0;
	for (it = _Locations.begin(); it != it2; it++)
	{
		std::cout << "Location name: " << it->name << std::endl;
		std::cout << "URL location: " << _Url << std::endl;
		if (_Url.find(it->name) != std::string::npos)
		{
			std::cout << "_Locindex length: " << _Locations[_Locindex].name.length() << " - it length: " << it->name.length() << std::endl;
			if (_Locations[_Locindex].name.length() < it->name.length())
				_Locindex = i;
		}
		i++;
	}
	std::cout << "Choosen location: " << _Locations[_Locindex].name << std::endl;
	_LocName = _Locations[_Locindex].name;
	this->_IndexPage = _Locations[_Locindex].index;
	if (_Locations[_Locindex].autoindex == 1)
		this->_Autoindex = "autoindex on";
	else
		this->_Autoindex = "autoindex off";
	this->_Sroot = _Locations[_Locindex].root; 
	std::cout << "_Sroot: " << this->_Sroot << std::endl;
	
	//this->_Sroot = "../www";

	std::cout << "AUTOINDEX: " << _Autoindex << std::endl;

	// this->_Url = this->_Url.substr(_Locations[_Locindex].name.length());
	std::cout << "Обрезанное имя " << this->_Url.substr(_Locations[_Locindex].name.length() - 1) << std::endl;
	CgiEnv* cgiEnv;
	std::cout << "this->_Method: " << this->_Method << std::endl;
	
	cgiEnv = this->createCgiEnv();
	std::cout << "cgi: " << (this->_Cgi ? "yes" : "no") << std::endl;
	if (this->_Cgi)
	{
		std::string cgi_path = this->_Locations[_Locindex].cgi_path;
		std::string cgi_ext = this->_Locations[_Locindex].cgi_extension[0];
		std::cout << "cgi_path: " << cgi_path << std::endl;
		std::cout << "cgi_ext: " << cgi_ext << std::endl;
		CgiServer* cgiServer = new CgiServer(this->_Config, this->_Url, _Locindex, 0, cgiEnv);
		HTTPprotocol httpResponse(newSocket, _Sroot, cgiServer->cgiResponse(), _404, "", 200);
		this->_Cgi = 0;
	}
	/*  Если метод Delete */
	else if (this->_Method == "DELETE")
	{
		char *  delName;
		HDE::Directory	dir(_404, _Sroot, _LocName);
		int isValidMethod = this->isValidMethod(this->_Method);
		if (isValidMethod)
		{
			delName = (char *)(_Sroot + this->_Url).c_str();
			// Если не можем отвязать, то выдаем 404, в других случаях 
			// передаем заголовки с content-length, равным 0
			// без тела
			if (unlink(delName) == -1)
				HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "", 404);
			else
				HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "del", 200);
		}
		else
		{
			this->_Url = _405;
			HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "notallowed", 405);
		}
	}
	else if (this->_Method == "POST")
	{
		struct stat buffer;
		int fd = -1;
		int isValidMethod = this->isValidMethod(this->_Method);
		
		if (isValidMethod)
		{
			std::cout << "Hello from POST" << std::endl;
			std::cout << _Buffer << std::endl;
			std::vector<std::string> params = splitSubstr(_Buffer, "\r\n\r\n");
			// std::cout << params[1] << std::endl;
			// std::cout << params[1].length() << std::endl;
			std::string path;
			if (this->_Url == "/")
				path = _Sroot + this->_Url + _IndexPage;
			else
				path = _Sroot + this->_Url;
			std::string contentLengthAr = params[0].substr(params[0].find("Content-Length:") - 1);
			std::vector<std::string> contentLengthAr1 = splitSubstr(contentLengthAr, "\r\n");
			std::vector<std::string> contentLengthAr2 = split(contentLengthAr1[0], ':');
			int contentLength = atoi(contentLengthAr2[1].c_str());
			if (contentLength > this->_Locations[0].client_max_body_size)
			{
				std::cout << "Entity too large" << std::endl;
				this->_Url = _413;
				std::cout << "this->_Url: " << this->_Url << std::endl;
				HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "large", 413);
			}
			else 
			{
				// std::cout << "path: " << path << std::endl;
				// Проверяем, существует ли файл по пути path
				// Если exist=0, то файл существует, иначе - нет
				int exist = stat(path.c_str(), &buffer);
				if (exist == 0 && params[1].length())
				{
					if ((fd = open(path.c_str(), O_WRONLY | O_TRUNC, 0644)) == -1)
					{
						std::cout << "TO CHANGE" << std::endl;
						exit (-1);
					}
					size_t w1 = write(fd, params[1].c_str(), params[1].length());
					if (w1  < 0)
					{
						std::cout << "sorry, I can`t write" << std::endl;
						exit (-1);
					}
					close(fd);
				}
				else
				{
					if ((fd = open(path.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
					{
						std::cout << "500 Error" << std::endl;
						exit (-1);
					}
					size_t w2 = write(fd, params[1].c_str(), params[1].length());
					if (w2  < 0)
					{
						std::cout << "sorry, I can`t write" << std::endl;
						exit (-1);
					}
					close(fd);
				}
				std::cout << "this->_Url: " << this->_Url << std::endl;
				HTTPprotocol httpResponse(newSocket, _Sroot, path, _404, "", 200);
			}
		}
		else
		{
			this->_Url = _405;
			HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "notallowed", 405);
		}
	}
	else
	{
		int isValidMethod = this->isValidMethod("GET");
		if (isValidMethod)
		{
			/*Блок обработки локейшн с автоиндексом*/
			if (_Autoindex == "autoindex on")
				this->responderAutoindexOn(newSocket);
			else
				this->responderAutoindexOff(newSocket);
		}
		else
		{
			this->_Url = _405;
			HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "notallowed", 405);
		}
	}
	// this->_Sroot = "../www";
}

void HDE::TestServer::responderAutoindexOn(int newSocket)
{
	HDE::Directory	dir(_404, _Sroot, _LocName);
	// Проверка результата. Можно позже убрать
	std::cout << "url test:" << this->_Url << std::endl;
	
	this->_Url = this->_Url.substr(_Locations[_Locindex].name.length() - 1);
	this->_Url = _Sroot + this->_Url;
	const char		*url_char = this->_Url.c_str();
	std::cout << "url_char" << url_char << std::endl;
	
	if ((dir.openDir((char*)url_char)) == 0)
		HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "", 200);
	else
	{
		std::string			add;

		if (!strcmp(this->_Url.c_str(), "/"))
			add = _IndexPage; //вместо названия будет передаваться параметр из конфига
		else
			add = "/" + _IndexPage;

		std::fstream	indexf(this->_Url + add);
		
		if (indexf.is_open() == 1)
		{
			indexf.close();
			this->_Url += add;
			HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "", 200);
		}
		else
		{
			indexf.close();
			HTTPprotocol httpResponse(newSocket, _Sroot, dir.makeLinks(dir.getPDir(), (char*)url_char), _404, "", 200);
		}
	}
}

void HDE::TestServer::responderAutoindexOff(int newSocket)
{
	HDE::Directory	dir(_404, _Sroot, _LocName);

	this->_Url = _Sroot + this->_Url;

	const char		*url_char = this->_Url.c_str();

	if (dir.openDir((char*)url_char) == 1)
	{
		if (!strcmp(this->_Url.c_str(), "/"))
			this->_Url += _IndexPage; //вместо названия будет передаваться параметр из конфига
		else
			this->_Url += "/" + _IndexPage;

		std::fstream		indexf(this->_Url);

		if (indexf.is_open() == 0)
		{
			this->_Url = _403;
		}
		indexf.close();
	}

	HTTPprotocol httpResponse(newSocket, _Sroot, _Url, _404, "", 200);
}

void	HDE::TestServer::add_client(int newSocket)
{
	pollfd		client_pollfd;
    

	std::cout << "Here(add)?" << std::endl;
	client_pollfd.fd = newSocket;
    client_pollfd.events = POLLIN;
    _Poll_sets.push_back(client_pollfd);
    
	printf("now_client_num: %u\n", (unsigned int)_Poll_sets.size()-1);
}

void HDE::TestServer::too_many_clients(int newSocket)
{
    printf("%s\n", ERR_MESSAGE);
    send(newSocket, ERR_MESSAGE, strlen(ERR_MESSAGE)+1, 0);
    close(newSocket);
}

void HDE::TestServer::del_client(std::vector<pollfd>::iterator it)
{
	close(it->fd);
    _Poll_sets.erase(it);
    printf("now_client_num: %u\n", (unsigned int)_Poll_sets.size()-1);
}

void HDE::TestServer::launch()
{
	pollfd		listener_fd[2]; // _Config->getServerSize()
	// AF_INET/*2*/, SOCK_STREAM/*1*/, 0, 80, "127.0.0.1", 10
	//HDE::ListeningSocket	ls(AF_INET/*2*/, SOCK_STREAM/*1*/, 0, 80, "127.0.0.2", 10);
	
	listener_fd[0].fd = getSocket()->getSock();
	listener_fd[0].events = POLLIN;
	_Poll_sets.push_back(listener_fd[0]);
	// listener_fd[1].fd = ls.getSock();
	// listener_fd[1].events = POLLIN;
	// _Poll_sets.push_back(listener_fd[1]);
	
	std::cout << "--------WAITING---------" << std::endl;
	
	while (1 < 2)
	{
		int ret = poll((pollfd *)&_Poll_sets[0], (unsigned int)_Poll_sets.size(), -1);
		// std::cout << "ret: " << ret << std::endl;
		// std::cout << "POLLIN: " << POLLIN << std::endl;
		// std::cout << "listener_fd.revents: " << _Poll_sets[0].revents << std::endl;
		if (ret == -1)
			perror("poll");
		else if(ret == 0)
			std::cout << "Timeout occured! No data after 3.5 seconds" << std::endl;
		else
		{	
			std::vector<pollfd>::iterator it;
			std::vector<pollfd>::iterator ite = _Poll_sets.end();
			
			for (it = _Poll_sets.begin(); it != ite; it++)
			{	
				if (it->revents & POLLIN)
				{
					std::cout << "--------WAITING---------" << std::endl;
					std::cout << "it->fd: " << it->fd << std::endl;
					if (it->fd == listener_fd[0].fd)
					{
						_NewSocket = acceptor();
						if (_NewSocket == -1)
						{
							std::cout << "Here?" << std::endl;
							perror("Accept Failed");
							continue; 
						}
						if (_Poll_sets.size() - 1 < MAX_CLIENT_NUM)
							add_client(_NewSocket);
						else
							too_many_clients(_NewSocket);
					}
					else
					{
						int read_size = read(it->fd, _Buffer, 30000);
						_Buffer[read_size] = '\0';
						if (read_size <= 0)
							del_client(it);
						else
						{
							it->revents = POLLOUT;
							handler();
							responder(it->fd);
							std::cout << "---------Done------------" << std::endl;
						}	
					}
				}
				else if (it->revents & POLLERR)
				{
					if (it->fd == listener_fd[0].fd)
					{
						perror("listen socket error");
						exit(1);
					}
					else
						del_client(it);
				}
			}
			
		}
	}	
}
