/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEnv.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hemelia <hemelia@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 23:32:48 by hemelia           #+#    #+#             */
/*   Updated: 2021/07/11 16:45:13 by hemelia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>

namespace HDE
{
	class CgiEnv
	{
		private:
				std::vector<std::string>	_PEnv;
				int 						_ParamNum;
				std::vector<std::string> 	_Params;
				std::string 				_QString;
				std::string 				_PostBody;
				std::string					_Method;
		public:
				CgiEnv();
				CgiEnv(const std::string qString, const std::string postBody,
				const std::string method);
				~CgiEnv();
				std::vector<std::string>	getPEnv();
				std::string					getPostBody();
				int 						getParamNum();
				void						setPEnv();
				void 						makeParamsMap(std::string params);
				std::vector<std::string> 	split(const std::string &s, char delim);
				CgiEnv 						&operator=(const CgiEnv &other);
				CgiEnv(CgiEnv const & cgiEnv);
	};
}
