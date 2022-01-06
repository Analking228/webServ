/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 23:03:50 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/21 17:55:00 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace HDE
{
	class CgiEnv;
	class Configuration;
	class CgiServer
	{
		private:
				std::string					_RequestBody;
				std::string					_RequestHeader;
				Configuration* 				_ServerConfig;
				CgiEnv*						_CgiEnv;
				std::string 				_ScriptUrl;
				int 						_Location;
				int 						_Server;
				char** 						_Args;
				char**						_PEnv;
				CgiServer();
		public:
				CgiServer(Configuration* serverConfig, std::string scriptUrl, int location, int server, CgiEnv* cgiEnv);
				~CgiServer();
				int spawnProcess(const char *const *args, const char * const *pEnv);
				std::string cgiResponse();
	};
}