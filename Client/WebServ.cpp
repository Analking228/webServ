#include "WebServ.hpp"
# include "../Server/TestServer.hpp"

//#include "./Parser/HeadersBlock.hpp"
//#include "./Parser/Configuration.hpp"


//char **g_envp;

HDE::Configuration test;

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
	HDE::Configuration* test = new HDE::Configuration(path);
	// test = HDE::Configuration(path);
	// test->print();
	std::string cgi_path = test->getServers()[0].locations[0].cgi_path;

	std::cout << "cgi_path: " << cgi_path << std::endl;
	// HDE::TestServer t;

	// HDE::TestServer	*ar[test->getServerSize()];

	// for (int i = 0, j = test->getServerSize(); i < j; i++)
	// {
	// 	ar[i] = new HDE::TestServer(test, i);
	// 	std::cout << "Server Index: " << i << std::endl;
	// }

	HDE::TestServer t(test);
	// std::cout << test.hhS.client_max_body_size << std::endl;
	// std::cout << this->_location.name << std::endl;
	// delete[] *ar;
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