#pragma once

namespace HDE
{
class HTTPprotocol
{
private:
	int				_Socket;

	int				_ContentLength;
	int				_ErrorCode;
	std::string		_404;
	std::string		_Header;
	std::string		_WorkDir;
	std::string 	_Url;
	std::string 	_Del;

public:
	HTTPprotocol(int socket, std::string workdir, std::string url, std::string nf404, std::string del, int ercode);

	int				contentLength();
	void			makeHead();
	std::string		stringStatus(int statuscode);
	void			contentLenght(std::ifstream* file);
	std::string		contentType();

	void			writeToSocket(std::ifstream* file);

	int				getSocket();
};	
}