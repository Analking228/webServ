#include "Parsing.hpp"

//Проверка на закрытие скобок
size_t getClosingBracket(std::string source, size_t line)
{
	size_t n;
	size_t size;
	size_t open_brackets;

	open_brackets = 0;
	if (getLine(source, line)[getLine(source, line).size() - 1] != '{')
		throw ParsingException(line, "Expected '{'.");
	n = line + 1;
	size = countLines(source);
	while (n < size)
	{
		if (!isSkippable(source, n) && endsWithOpenBracket(source, n))
			++open_brackets;
		if (!isSkippable(source, n) && getLine(source, n) == "}")
		{
			if (open_brackets == 0)
				return (n);
			--open_brackets;
		}
		++n;
	}
	throw ParsingException(line, "Expected '}'.");
	return (-1);
}

// Проверка на наличии '{' как последний элемент строки
bool endsWithOpenBracket(std::string source, size_t line)
{
	std::vector<std::string> splits;

	splits = splitWhitespace(getLine(source, line));
	if (splits.size() > 0)
	{
		if (splits[splits.size() - 1] == "{")
			return (true);
	}
	return (false);
}

// читаем файл
std::string readFile(std::string file)
{
	char buffer[BUFFER_SIZE + 1] = {0};
	int fd;
	int i;
	int res;
	std::string result;

	fd = open(file.c_str(), O_RDONLY);
	if (fd < -1)
	{
		std::cout << "Error" << std::endl;
		throw ParsingException(0, "The file " + file + " does not exists.");
	}
	while ((res = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		result += buffer;
		i = 0;
		while (i < BUFFER_SIZE)
			buffer[i++] = 0;
	}
	if (res < 0)
		throw ParsingException(0, "Error while reading " + file + ".");
	close(fd);
	return (result);
}

// Читаем файл и помещаем в вектор
std::vector<unsigned char> readBinaryFile(std::string file)
{
	char buffer[BUFFER_SIZE + 1] = {0};
	int fd;
	int i;
	int res;
	std::vector<unsigned char> result;

	fd = open(file.c_str(), O_RDONLY);
	if (fd < -1)
	{
		std::cout << "Error" << std::endl;
		throw ParsingException(0, "The file " + file + " does not exists.");
	}
	while ((res = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		for (size_t j = 0; j < (size_t)res; ++j)
			result.push_back(buffer[j]);
		i = 0;
		while (i < BUFFER_SIZE)
			buffer[i++] = 0;
	}
	if (res < 0)
		throw ParsingException(0, "Error while reading " + file + ".");
	close(fd);
	return (result);
}

// Разбиваем линии, убираем пробелы
std::string getLine(std::string source, size_t n)
{
	size_t i;
	size_t j;
	size_t line_count;

	if (n >= countLines(source))
		return (std::string());
	line_count = 0;
	i = 0;
	while (line_count < n)
	{
		if (source[i++] == '\n')
			++line_count;
	}
	while (std::isspace(source[i]) && source[i] != '\n')
		++i;
	j = 0;
	while (source[i + j] && source[i + j] != '\n')
		++j;
	while (j > 0 && std::isspace(source[i + j - 1]))
		--j;
	return (std::string(source, i, j));
}

// Счет строк в файле.
size_t countLines(std::string source)
{
	size_t i;
	size_t lines;

	i = 0;
	lines = 1;
	while (source[i])
	{
		if (source[i++] == '\n')
			++lines;
	}
	return (lines);
}

// Проверка свойств сервека, как они отфарматированы
//"listen 80 localhost;" забивает в тектор на 3 -> "listen", "80" and "localhost"
std::vector<std::string> parseProperty(std::string source, size_t line, std::string object)
{
	std::vector<std::string> result;
	std::string l;

	l = getLine(source, line);
	if (l[l.size() - 1] != ';')
		throw ParsingException(line, "Expected ';'.");
	l = std::string(l, 0, l.size() - 1);
	result = splitWhitespace(l);
	if (result.size() <= 1)
		throw ParsingException(line, "Properties should have at least one value.");
	if (!isPropertyNameValid(result[0], object == "server" ? server_properties : route_properties))
		throw ParsingException(line, "'" + result[0] + "' is not a valid property for " + object + ".");
	return (result);
}

// Разделяем слова пробелом
std::vector<std::string> splitWhitespace(std::string str)
{
	std::vector<std::string> res;
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (std::isspace(str[i]))
		{
			if (i == j)
				++j;
			else
			{
				res.push_back(std::string(str, j, i - j));
				j = i + 1;
			}
		}
		++i;
	}
	if (i != j)
		res.push_back(std::string(str, j, i - j));
	return (res);
}

//Проверка наличия имени внутри списка
//"listen" в случае {"server_name", "listen", 0} будет истиной
//"hello" в {"server_name", "listen", 0} будет ложью 
bool isPropertyNameValid(std::string name, const char **valid_names)
{
	size_t i;

	i = 0;
	while (valid_names[i])
	{
		if (name == valid_names[i])
			return (true);
		++i;
	}
	return (false);
}

// Проверка линии : пустая или начинается с #
bool isSkippable(std::string source, size_t line)
{
	std::string l;

	l = getLine(source, line);
	return (splitWhitespace(l).size() == 0 || l.size() == 0 || l[0] == '#');
}

// Перевод string в integer
size_t uIntegerParam(std::string param, size_t line)
{
	size_t value;
	std::istringstream convert(param);

	if (!(convert >> value))
		throw ParsingException(line, "'" + param + "' is not a positive integer.");
	if (value < 0)
		throw ParsingException(line, "'" + param + "' is not a positive integer.");
	return (value);
}

// Определение метода
bool isMethodValid(std::string method)
{
	size_t i;

	i = 0;
	while (methods[i])
	{
		if (methods[i] == method)
			return (true);
		++i;
	}
	return (false);
}

// истина или лож
bool boolParam(std::string param, size_t line)
{
	if (param == "on")
		return (true);
	else if (param == "off")
		return (false);
	else
		throw ParsingException(line, "Boolean parameter should be \"on\" or \"off\".");
}

// переводим size_t в std::string
std::string uIntegerToString(size_t n)
{
	std::ostringstream convert;

	convert << n;
	return (convert.str());
}

// Замена всех входящих в ::string
std::string replace(std::string source, std::string to_replace, std::string new_value) {
	size_t start_pos = 0;
	while((start_pos = source.find(to_replace, start_pos)) != std::string::npos) {
		source.replace(start_pos, to_replace.length(), new_value);
		start_pos += new_value.length();
	}
	return (source);
}

// Проверка path(пути) в файле
int pathType(std::string path, time_t *file_date)
{
	struct stat buffer;
	struct timezone tz;
	struct timeval t;

	gettimeofday(&t, &tz);
	int exist = stat(path.c_str(), &buffer);
	if (file_date)
		*file_date = buffer.st_mtime + tz.tz_minuteswest * 60;
	if (exist == 0)
	{
		if (S_ISREG(buffer.st_mode))
			return (1);
		else
			return (2);
	}
	else
		return (0);
}

std::string itoa(int nb)
{
	std::string s;
	std::stringstream out;
	out << nb;
	s = out.str();
	return (s);
}