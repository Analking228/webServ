/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directory.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 18:14:12 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/22 12:50:40 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORY_HPP
# define DIRECTORY_HPP

#include <dirent.h>

namespace HDE
{
	class Directory
	{
		private:
				struct dirent 	*_pDirent;
				DIR 			*_pDir;
				char			*_dirName;
				int				_dirNotFound;
				std::string		_404;
				std::string		_SRoot;
				std::string 	_LocName;
					
		public:
				Directory(std::string nf404, std::string sRoot, std::string locName);
				~Directory();
				int				openDir(char *dirName);
				std::string		makeLinks(DIR *pDir, char *dirName);
				DIR 			*getPDir();	
	};
};
# endif
