/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/12 16:39:35 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/22 15:27:12 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ASimpleServer.hpp"
#include <vector>
#include <map>
#include <poll.h>
#include "../CGI/CgiEnv.hpp"
#include "../Client/Parser/Configuration.hpp"
#include "../CGI/CgiServer.hpp"

namespace HDE
{
	class TestServer : public ASimpleServer
	{
	private:
		char*									_Buffer;
		int										_NewSocket;
		int										_ServerIndex;
		std::string								_Autoindex;	
		std::string								_Sroot;
		std::string   							_Url;
		std::string								_Method;
		std::map<std::string, std::string> 		_Params;
		std::vector <pollfd> 					_Poll_sets;
		HDE::Configuration* 					_Config;
		HDE::CgiServer* 						_CgiServer;
		int 									_Cgi;
		u_long									_MaxBodySize;
		std::string								_ServerName;
		std::string								_IndexPage;
		std::string								_404;
		std::string								_403;
		std::string 							_413;
		std::string 							_405;
		std::vector<Configuration::location> 	_Locations; // берем ЛОКЕЙШН из конфига сервера, но структуру сервера не берем
		int 									_Locindex;
		std::string 							_LocName;

	public:			
		int							acceptor();
		void						findRoot();
		void						handler();
		void						responder(int newSocket);
		void 						responderAutoindexOn(int newSocket);
		void						responderAutoindexOff(int newSocket);
		void						ejectConfData();
		void						responseBody(int type, std::string root);
		void 						add_client(int newSocket);
		void 						too_many_clients(int newSocket);
		void 						del_client(std::vector<pollfd>::iterator it);
		std::vector<std::string> 	split(const std::string &s, char delim);
		std::vector<std::string>	splitSubstr(const std::string &s, std::string delimiter);
		void 						makeParamsMap(std::vector<std::string> params);
		HDE::CgiEnv* 				createCgiEnv();
		int 						isValidMethod(std::string method);
		TestServer();
		~TestServer();
		TestServer(HDE::Configuration *config);
		TestServer(HDE::Configuration *config, int ServIndex);
		void	launch();
	};
}
