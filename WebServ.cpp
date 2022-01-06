#include "WebServ.hpp"
//#include "./Parser/HeadersBlock.hpp"
//#include "./Parser/Configuration.hpp"


//char **g_envp;

Configuration test;




int main(int argc, char **argv/*, char **env*/)
{
	Log("Start Server(s)...");


	char *path;

	//g_envp = env;

	char def[] = "./Config/confdef.conf";

	if (argc != 2)
	{
		//std::cout << "Please use : ./WebServ <path>" << std::endl;
		path = def;
		//return (1);
	}
	else
		path = argv[1];
	test = Configuration(path);
	test.print();
	//std::cout << test.hhS.client_max_body_size << std::endl;
	//std::cout << this->_location.name << std::endl;
	return (0);
}

void throwError(const std::exception& ex)
{
	std::cerr << "\033[1m\033[31mERROR \033[0m: " << ex.what() << '\n';
}

void outError(const std::string& msg)
{
	std::cerr << "\033[1m\033[31mERROR \033[0m: " << msg << '\n';
}