/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdelibc-server.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 21:57:56 by hemelia           #+#    #+#             */
/*   Updated: 2021/06/22 21:01:47 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>
#include <sys/stat.h>

#define CRLF "\r\n"

#include "ASimpleServer.hpp"
#include "Directory.hpp"
#include "HttpSpecialResponse.hpp"
#include "HTTPprotocol.hpp"
#include "TestServer.hpp"
