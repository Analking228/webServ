/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 23:03:53 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/22 07:03:56 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "CgiServer.hpp"
#include "CgiEnv.hpp"
#include "../Client/Parser/Configuration.hpp"

HDE::CgiServer::~CgiServer()
{
	if (this->_PEnv)
	{
		int i = 0;
		while (this->_PEnv[i])
			free(this->_PEnv[i]);
		free(this->_PEnv);
	}
	if (this->_Args)
	{
		int i = 0;
		while(this->_Args[i])
			free(this->_Args[i]);
		free(this->_Args);
	}	
}

HDE::CgiServer::CgiServer(HDE::Configuration* serverConfig, std::string scriptUrl, int location, int server, HDE::CgiEnv* cgiEnv) 
: _ServerConfig(serverConfig), _ScriptUrl(scriptUrl), _Location(location), _Server(server), _CgiEnv(cgiEnv)
{
	// Записываем в первый аргумент путь к интерпретатору, во второй путь к скрипту,
	// третий зануляем
	static const std::string strCgiPath = serverConfig->getServers()[server].locations[location].cgi_path;
	static const std::string strScriptUrl = "../www/" + scriptUrl;
	this->_Args = (char**)malloc(sizeof(char*) * 3);
	if (this->_Args)
	{
		// Надо ли под каждый _Args выделать еще память malloc
		this->_Args[0] = (char*)strCgiPath.c_str(); 
		this->_Args[1] = (char*)strScriptUrl.c_str();
		this->_Args[2] = (char*)"0";
	}
	int i = 0;
	while (i < 3)
	{
		std::cout << "Arg[" << i << "] = " << 	this->_Args[i] << std::endl;
		i++;
	}
	//  Формируем тело запроса и его длину
	this->_RequestBody = this->_CgiEnv->getPostBody();
	std::cout << "RequestBody: " << this->_RequestBody << std::endl;
	this->_RequestHeader = "Content-Length=" + std::to_string((long long)this->_RequestBody.length());
	std::cout << "RequestHeader: " << this->_RequestHeader << std::endl;
	// 
	int paramNum = this->_CgiEnv->getParamNum();
	std::cout << "paramNum: " << paramNum << std::endl;
	this->_PEnv = (char**)malloc(sizeof(char*) * (paramNum + 1));
	if (this->_PEnv)
	{
		// Надо ли под каждый _PEnv выделать еще память malloc
		int i = 0;
		while (i < paramNum)
		{
			// this->_PEnv[i] = (char*)malloc(sizeof(char) * (this->_CgiEnv->getPEnv()[i].length() + 1));
			this->_PEnv[i] = strdup(this->_CgiEnv->getPEnv()[i].c_str());
			std::cout << "Penv[" << i << "] = " << 	this->_PEnv[i] << std::endl;
			i++;
		}
		this->_PEnv[i] = (char*)"0";
	}
	return ;
}

int HDE::CgiServer::spawnProcess(const char *const *args, const char * const *pEnv)
{
    int pid = fork();
	std::cout << "Test pid: " << pid << std::endl;
    if(pid == 0)
    {
		std::cout << execl (args[0], args[0], args[1]);
		exit(EXIT_SUCCESS);
	}
    return pid;  
}

std::string HDE::CgiServer::cgiResponse()
{
	pid_t pid;
	int tmp_fd;
	int fd[2];
	int exec_res;
	
	if (pipe(fd) == -1)
	{
		std::cout << "Cannot create CGI pipe" << std::endl;
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		tmp_fd = open("../tmp/dir_tmp.html", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if (tmp_fd < 0)
		{
			std::cout << "Cannot create temporary file to catch CGI output in /tmp." << std::endl;
			return (NULL);
		}
		dup2(tmp_fd, 1);
		dup2(tmp_fd, 2);
		exec_res = execl(this->_Args[0], this->_Args[0], this->_Args[1]);
		close(0);
		close(tmp_fd);
		close(fd[0]);
		exit(0);
	}
	else
	{
		close(fd[0]);
		write(fd[1], this->_RequestBody.c_str(), this->_RequestBody.length());
		close(fd[1]);
		waitpid(-1, NULL, 0);
	}
	return (std::string("../tmp/dir_tmp.html"));;
}
