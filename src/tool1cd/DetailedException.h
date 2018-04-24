/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TOOL1CD_PROJECT_DETAILEDEXCEPTION_H
#define TOOL1CD_PROJECT_DETAILEDEXCEPTION_H

#include <exception>
#include <System.Classes.hpp>

class DetailedException : public std::exception {
public:
	DetailedException(const std::string &message) throw();
	DetailedException(const DetailedException &src) throw();

	void show() const throw();
	DetailedException &add_detail(const std::string &paramName, const std::string &paramValue) throw();

	DetailedException &add_detail(const std::string &paramName, const char *paramValue) throw();

	template <typename value_type>
	DetailedException &add_detail(const std::string &paramName, value_type paramValue) throw()
	{
		return add_detail(paramName, std::to_string(paramValue));
	}

	virtual const char *what() const throw() override ;

private:
	std::string message;
	TStringList params;
};


class FieldStreamParseException : public DetailedException
{
public:
	FieldStreamParseException(const std::string &message) throw();
};

class SerializationException : public DetailedException
{
public:
	SerializationException(const std::string &message) throw();
};

class PackDirectoryDoesNotExistException : public DetailedException
{
public:
	 PackDirectoryDoesNotExistException(const std::string &message) throw();
};

class SupplierConfigReadException : public DetailedException
{
public:
	SupplierConfigReadException(const std::string &message) throw();
};

#endif //TOOL1CD_PROJECT_DETAILEDEXCEPTION_H
