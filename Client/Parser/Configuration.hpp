#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <sstream>
# include "Parsing.hpp"

namespace HDE
{
	class Configuration
	{
		public:
			/*struct X
			{
				int l = 5;
			};
			X x;*/
			struct location
			{
				std::string name; //название после / в конфиге
				std::string root; // корневая папка при запросе на локейшн
				std::vector<std::string> methods; // методы доступные для локешн
				bool autoindex; 
				std::string index;
				std::vector<std::string> cgi_extension; // какой СГА используется
				std::string cgi_path; //  и папка с которой берутся файлы для обработки
				bool upload_enable; // доступность загрузки и метода ?
				std::string upload_path; // путь загрузки
				size_t client_max_body_size; // макс длинна тела клиентского запроса
			};
			//location hhS;
			struct server
			{
				std::vector<std::string> names;
				std::string host;
				std::string root;
				std::map<int, std::string> error_pages;
				std::vector<location> locations;
				size_t port;
			};
		private:
			std::vector<server> _servers;
			void _parseConfig(std::string file);
			void _parseServer(std::string source, size_t line_start, size_t line_end);
			void _parseServerProperty(std::string source, size_t line, server &s);
			void _parseLocationProperty(std::string source, size_t line, location &l);
			void _validateConfig(void);
			void _completeConfig(void);
			server _defaultServer(void);
			location _defaultLocation(void);
			location _parseLocation(std::string source, size_t line_start, size_t line_end);
		public:
			Configuration(void);
			Configuration(std::string file);
			Configuration &operator=(const Configuration &other);
			~Configuration(void);
			std::vector<server> getServers(void);
			std::map<int, std::string> getError_pages(size_t i);
			std::vector<location> getLocations(size_t i);
			int getServerSize(void);
			HDE::Configuration::server getServerIterator(size_t i);
			void print(void);
	};
};

#endif