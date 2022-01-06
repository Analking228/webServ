/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEnv.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 23:32:55 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/21 08:00:31 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include <sstream>
#include "CgiEnv.hpp"

HDE::CgiEnv::CgiEnv(std::string qString, std::string postBody,
				std::string method) : _QString(qString), _PostBody(postBody),
				_Method(method), _ParamNum(0)
				{
					return ;
				}

HDE::CgiEnv::CgiEnv() : _ParamNum(0)
{
	this->_Method = "";
	this->_PostBody = "";
	this->_QString = "";
	return ;
}
HDE::CgiEnv &HDE::CgiEnv::operator=(const HDE::CgiEnv &other)
{
	this->_QString = other._QString;
	this->_PostBody = other._PostBody;
	this->_Method = other._Method;
	if (other._ParamNum)
		this->_ParamNum = other._ParamNum;
	else
		this->_ParamNum = 0;
	return (*this);
}

HDE::CgiEnv::CgiEnv(HDE::CgiEnv const & cgiEnv)
{
	if (cgiEnv._ParamNum)
		this->_ParamNum = cgiEnv._ParamNum;
	else
		this->_ParamNum = 0;
	*this = cgiEnv;
	return;
}

HDE::CgiEnv::~CgiEnv()
{
	return;
}

std::vector<std::string> HDE::CgiEnv::split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void HDE::CgiEnv::makeParamsMap(std::string params)
{
	if (params.size() > 1)
	{
		std::cout << "params: " << params << std::endl;
		this->_Params = split(params, '&');
		this->_ParamNum = this->_Params.size() + 12;
	}
}

std::vector<std::string> HDE::CgiEnv::getPEnv()
{
	return (this->_PEnv);
}

int HDE::CgiEnv::getParamNum()
{
	return (this->_ParamNum);
}

std::string	 HDE::CgiEnv::getPostBody()
{
	return (this->_PostBody);
}

void	HDE::CgiEnv::setPEnv()
{
	if (this->_Method == "POST")
		makeParamsMap(this->_PostBody);
	else if (this->_Method == "GET")
		makeParamsMap(this->_QString);
	std::string PostHeader = "CONTENT_LENGTH=" + std::to_string((long long)(this->_PostBody.length()));
	std::string QString = "QUERY_STRING=" + this->_QString;
	std::string GatewayInterface = "GATEWAY_INTERFACE=CGI/1.1";
	std::string Method = "REQUEST_METHOD=" + this->_Method;
	std::string PathTranslated = "PATH_TRANSLATED=../www/profile.php"; // путь к файлу со скриптом, спроецированный на файловую систему
	std::string PathInfo = "PATH_INFO=/profile.php";  // заменить на исполняемый файл
	
	std::string ScriptName = "SCRIPT_NAME=/"; // возможно, не нужно будет использовать
	std::string ScriptFilename = "SCRIPT_FILENAME=profile.php"; // заменить на исполняемый файл
	std::string ServerName = "SERVER_NAME=localhost";  // заменить на имя сервера
	std::string ServerPort = "SERVER_PORT=8080"; // заменить на порт
	std::string ServerProtocol = "SERVER_PROTOCOL=HTTP/1.1";
	std::string ServerSoftware = "SERVER_SOFTWARE=ours";
	std::cout << "this->_ParamNum: " <<  this->_ParamNum  << std::endl;

	this->_PEnv.push_back(PostHeader);
	this->_PEnv.push_back(QString);
	this->_PEnv.push_back(GatewayInterface);
	this->_PEnv.push_back(Method);
	this->_PEnv.push_back(PathTranslated);
	this->_PEnv.push_back(PathInfo);
	this->_PEnv.push_back(ScriptName);
	this->_PEnv.push_back(ScriptFilename);
	this->_PEnv.push_back(ServerName);
	this->_PEnv.push_back(ServerPort);
	this->_PEnv.push_back(ServerProtocol);
	this->_PEnv.push_back(ServerSoftware);

	std::cout << "this->_PEnv[0]: " << this->_PEnv[0] << std::endl;
	std::cout << "this->_PEnv[1]: " << this->_PEnv[1] << std::endl;
	std::cout << "this->_PEnv[2]: " << this->getPEnv()[2] << std::endl;
	std::cout << "this->_PEnv[3]: " << this->getPEnv()[3] << std::endl;
	std::cout << "this->_PEnv[4]: " << this->_PEnv[4] << std::endl;
	std::cout << "this->_PEnv[5]: " << this->_PEnv[5] << std::endl;
	std::cout << "this->_PEnv[6]: " << this->_PEnv[6] << std::endl;
	std::cout << "this->_PEnv[7]: " << this->_PEnv[7] << std::endl;
	std::cout << "this->_PEnv[8]: " << this->_PEnv[8] << std::endl;
	std::cout << "this->_PEnv[9]: " << this->_PEnv[9] << std::endl;
	std::cout << "this->_PEnv[10]: " << this->_PEnv[10] << std::endl;
	std::cout << "this->_PEnv[11]: " << this->_PEnv[11] << std::endl;

	int i = 12;
	while (i < this->_ParamNum)
	{
		this->_PEnv.push_back(this->_Params[i - 12]);
		std::cout << "this->_PEnv["<< i <<"]: " << this->_PEnv[i] << std::endl;
		i++;
	}
}
