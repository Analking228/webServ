/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directory.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 18:14:14 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/22 13:08:50 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>

#include "Directory.hpp"
#include "HttpSpecialResponse.hpp"

#define CRLF "\r\n"

HDE::Directory::Directory(std::string nf404, std::string sRoot, std::string locName) : _404(nf404), _SRoot(sRoot),
_LocName(locName)
{
	this->_pDir = NULL;
	this->_dirName = NULL;
	this->_pDirent = NULL;
	this->_dirNotFound = 0;
}

HDE::Directory::~Directory()
{
	if (_pDir)
		closedir (_pDir);
}

int		HDE::Directory::openDir(char *dirName)
{
	this->_pDir = opendir(dirName);
	if (_pDir == NULL) {
		this->_dirNotFound = 1;
		return (0);
	}
	return (1);
}

DIR * HDE::Directory::getPDir()
{
	return (this->_pDir);
}

std::string	HDE::Directory::makeLinks(DIR *pDir, char *dirName)
{
	std::ofstream links("../tmp/dir_tmp.html");
	
	// Отрезаем от текущей директории "../www" (_SRoot)
	std::string currentDirName = std::string(dirName);
	currentDirName = currentDirName.substr(_SRoot.length() + 1);
	
	std::cout << "currentDirName: " << currentDirName << std::endl;
	
	// Добавляем в ссылку имя текущей директории currentDirName
	// Тонкий момент. Если находимся в корневой директории то не нужно добавлять /
	// Если находимся ниже, то без слеша неправильно работает. Надо разобраться
	// Пока костыльно добавила слеш std::string slash = "/";
	if (this->_dirNotFound)
		return (std::string(_404));
	else
	{
		std::cout << "currentDirName: " << currentDirName << std::endl;
		links << "<html>" << CRLF << "<head><title>" << dirName
		<< "</title></head>" << CRLF << "<body>" << CRLF;
		std::string slash = "/";
		// if (currentDirName.length() && currentDirName[0] != '/')
		// 	currentDirName = slash.append(currentDirName);
		std::cout << "currentDirName2: " << currentDirName << std::endl;
		std::cout << "_LocName: " << _LocName << std::endl;
		while ((this->_pDirent = readdir(pDir)) != NULL) 
		{
			std::cout << "this->_pDirent->d_name: " << this->_pDirent->d_name  << std::endl;
			
			if (strcmp(this->_pDirent->d_name, ".") && strcmp(this->_pDirent->d_name, ".."))
				links << "<a href=\"" << _LocName << currentDirName << "/" << this->_pDirent->d_name << "\">"
				<< this->_pDirent->d_name << "</a><br>";
		}
		links << "</body>" << CRLF << "</html>";
	}
	links.close();

	return (std::string("../tmp/dir_tmp.html"));
}
