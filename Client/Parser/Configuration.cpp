#include "Configuration.hpp"

HDE::Configuration::Configuration(void)
{}

// Чтение конфига -> заполнение структуры -> передача структуры
HDE::Configuration::Configuration(std::string file)
{
	_parseConfig(file);
	_completeConfig();
}

HDE::Configuration &HDE::Configuration::operator=(const Configuration &other)
{
	this->_servers = other._servers;
	return (*this);
}

HDE::Configuration::~Configuration(void)
{}

// Парсим полученный файл. Открываем и разбиваем строки
void HDE::Configuration::_parseConfig(std::string file)
{
	std::string file_content;
	size_t i;
	size_t size;
	std::vector<std::string> line;

	i = 0;
	file_content = readFile(file);
	//std::cout << file_content << std::endl;
	size = countLines(file_content);
	while (i < size)
	{
		if (!isSkippable(file_content, i))
		{
			line = splitWhitespace(getLine(file_content, i));
			if (line.size() > 0 && line[0] == "server")
			{
				_parseServer(file_content, i, getClosingBracket(file_content, i));
				i = getClosingBracket(file_content, i);
			}
			else
				throw ParsingException(i, "Unexpected token '" + line[0] + "'.");
		}
		++i;
	}
	_validateConfig();
}

// Добавляем в структуру сервера параметры
void HDE::Configuration::_parseServer(std::string source, size_t line_start, size_t line_end)
{
	server s;
	std::vector<std::string> line;

	s = _defaultServer();
	for (size_t n = line_start + 1; n < line_end; ++n)
	{
		if (!isSkippable(source, n))
		{
			std::vector<std::string> words = splitWhitespace(getLine(source, n));
			if (words.size() > 0 && words[0] == "location")
			{
				s.locations.push_back(_parseLocation(source, n, getClosingBracket(source, n)));
				n = getClosingBracket(source, n);
			}
			else
				_parseServerProperty(source, n, s);
		}
	}
	_servers.push_back(s);
}

//Строку разделяем пробелами и чтение параметров
void HDE::Configuration::_parseServerProperty(std::string source, size_t n, server &s)
{
	std::vector<std::string> line;
	
	line = parseProperty(source, n, "server");
	if (line[0] == server_properties[0])
	{
		if (line.size() != 3)
			throw ParsingException(n, std::string(server_properties[0]) + " <port> <host>;");
		s.port = uIntegerParam(line[1], n);
		s.host = line[2];
	}
	if (line[0] == server_properties[1])
	{
		for (size_t i = 1; i < line.size(); ++i)
			s.names.push_back(line[i]);
	}
	if (line[0] == server_properties[2])
	{
		if (line.size() != 3)
			throw ParsingException(n, std::string(server_properties[2]) + "<code> <file>;");
		s.error_pages[uIntegerParam(line[1], n)] = line[2];	
	}
	if (line[0] == server_properties[3])
	{
		s.root = line[1];
	}
}

// создаем и передаем структуру location
HDE::Configuration::location HDE::Configuration::_parseLocation(std::string source, size_t line_start, size_t line_end)
{
	location loc;
	std::vector<std::string> line;

	loc = _defaultLocation();
	line = splitWhitespace(getLine(source, line_start));
	if (line.size() != 3)
		throw ParsingException(line_start, "Location should have a name.");
	loc.name = line[1];
	for (size_t n = line_start + 1; n < line_end; ++n)
	{
		if (!isSkippable(source, n))
			_parseLocationProperty(source, n, loc);
	}
	return (loc);
}

//Строку со свойствами переносим в структуру locaition
void HDE::Configuration::_parseLocationProperty(std::string source, size_t n, location &l)
{
	std::vector<std::string> line;
	char last;

	line = parseProperty(source, n, "route");
	if (line[0] == route_properties[0])
	{
		for (size_t i = 1; i < line.size(); ++i)
		{
			if (!isMethodValid(line[i]))
				throw ParsingException(n, "'" + line[i] + "' is not a valid method.");
			else
				l.methods.push_back(line[i]);
		}
	}
	if (line[0] == route_properties[1])
		l.root = line[1];
	if (line[0] == route_properties[2])
		l.autoindex = boolParam(line[1], n);
	if (line[0] == route_properties[3])
		l.index = line[1];
	if (line[0] == route_properties[4])
	{
		for (size_t i = 1; i < line.size(); ++i)
		l.cgi_extension.push_back(line[i]);
	}
	if (line[0] == route_properties[5])
		l.cgi_path = line[1];
	if (line[0] == route_properties[6])
		l.upload_enable = boolParam(line[1], n);
	if (line[0] == route_properties[7])
		l.upload_path = line[1];
	if (line[0] == route_properties[8])
	{
		if (line.size() != 2)
			throw ParsingException(n, std::string(server_properties[3]) + " <size[K,M,G]>;");
		l.client_max_body_size = uIntegerParam(line[1], n);
		last = line[1][line[1].size() - 1];
		if (last == 'K' || last == 'k')
			l.client_max_body_size *= 1024;
		else if (last == 'M' || last == 'm')
			l.client_max_body_size *= 1024 * 1024;
		else if (last == 'G' || last == 'G')
			l.client_max_body_size *= 1024 * 1024 * 1024;
		else if (!std::isdigit(last))
			throw ParsingException(n, std::string(server_properties[3]) + " <size[K,M,G]>;");
	}
}

// Вывод прочтенных данных из файла конфига
void HDE::Configuration::print(void)
{
	//std::map<int, std::string>::iterator it;
	std::vector<location>::iterator it2;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "- Server" << std::endl;
		std::cout << "   * server_name: ";
		for (size_t j = 0; j < _servers[i].names.size(); ++j)
			std::cout << _servers[i].names[j] << " ";
		std::cout << std::endl;
		std::cout << "   * host: " + _servers[i].host << std::endl;
		std::cout << "   * port: " + uIntegerToString(_servers[i].port) << std::endl;
		std::cout << "   * root: " + _servers[i].root << std::endl;
		//it = _servers[i].error_pages.begin();
		/*while (it != _servers[i].error_pages.end())
		{
			std::cout << "   * error_page for " + uIntegerToString(it->first) + ": " + it->second << std::endl;
			++it;
		}*/
		it2 = _servers[i].locations.begin();
		while (it2 != _servers[i].locations.end())
		{
			std::cout << "   - Location " + it2->name << std::endl;
			std::cout << "     * methods: ";
			for (size_t j = 0; j < it2->methods.size(); ++j)
				std::cout << it2->methods[j] + " ";
			std::cout << std::endl;
			std::cout << "     * root: " << it2->root << std::endl;
			std::cout << "     * cgi_extension: ";
			for (size_t j = 0; j < it2->cgi_extension.size(); ++j)
				std::cout << it2->cgi_extension[j] << " ";
			std::cout << std::endl;
			std::cout << "     * cgi_path: " << it2->cgi_path << std::endl;
			std::cout << "     * autoindex: " << it2->autoindex << std::endl;
			std::cout << "     * upload_enable: " << it2->upload_enable << std::endl;
			std::cout << "     * upload_path: " << it2->upload_path << std::endl;
			std::cout << "     * client_max_body_size: " + uIntegerToString(it2->client_max_body_size) << std::endl;
			++it2;
		}
	}
};

// Получение полного вектора сервера из конфига
std::vector<HDE::Configuration::server> HDE::Configuration::getServers(void) 
{
	return (_servers);
}

std::map<int, std::string> HDE::Configuration::getError_pages(size_t i)
{
	return(_servers[i].error_pages);
}

// Получение параметров конкретного сервера
HDE::Configuration::server HDE::Configuration::getServerIterator(size_t i)
{
	/*std::cout << _servers[i].host << "this is test for server.host" << std::endl; // проверка вывода*/
	return (_servers[i]);
}

// Получение location конкретного сервера
std::vector<HDE::Configuration::location> HDE::Configuration::getLocations(size_t i)
{
	return (_servers[i].locations);
}

// Определение количества серверов 
int HDE::Configuration::getServerSize(void)
{
	return (_servers.size());
}

// Проверка правильности работы парсера
void HDE::Configuration::_validateConfig(void)
{
	if (_servers.size() == 0)
		throw ParsingException(0, "Your configuration file must provide at least one server.");
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		for (size_t j = 0; j < _servers[i].locations.size(); ++j)
		{
			if (_servers[i].locations[j].cgi_path.size() > 0 &&
			pathType(_servers[i].locations[j].cgi_path, 0) != 1)
				throw ParsingException(0, "The cgi path '" + _servers[i].locations[j].cgi_path + "' is not a valid file.");
		}
	}
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		for (size_t j = 0; j < _servers.size(); ++j)
		{
			if (i != j)
			{
				if (_servers[i].host == "127.0.0.1")
					_servers[i].host = "localhost";
				if (_servers[j].host == "127.0.0.1")
					_servers[j].host = "localhost";
				if (_servers[i].host == _servers[j].host && _servers[i].port == _servers[j].port)
					throw ParsingException(0, "Two servers have the same host and port.");
			}
		}
	}
}

// Передаем дефолтную структуру
HDE::Configuration::server HDE::Configuration::_defaultServer(void)
{
	server s;

	s.port = 80;
	s.host = "127.0.0.1";
	s.root = "../www";
	return (s);
}

// Передаем дефолтные параметры location
HDE::Configuration::location HDE::Configuration::_defaultLocation(void)
{
	location l;

	l.name = "/";
	l.root = "";
	l.index = "";
	l.autoindex = false;
	l.cgi_path = "";
	l.upload_enable = false;
	l.upload_path = "";
	l.client_max_body_size = 1048576;
	return (l);
}

// Дополнение неполного конфига
void HDE::Configuration::_completeConfig(void)
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		if (_servers[i].locations.size() == 0)
			_servers[i].locations.push_back(_defaultLocation());
		for (size_t j = 0; j < _servers[i].locations.size(); ++j)
		{
			if (_servers[i].locations[j].methods.size() == 0)
				_servers[i].locations[j].methods.push_back("GET");
			if (_servers[i].locations[j].root.size() == 0)
				_servers[i].locations[j].root = _servers[i].root;
		}
	}
}