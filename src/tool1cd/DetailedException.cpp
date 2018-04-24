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

#include "DetailedException.h"
#include "MessageRegistration.h"

extern Registrator msreg_g;

void DetailedException::show() const throw()
{
	msreg_g.AddDetailedMessage(message, MessageState::Error, &params);
}

DetailedException &DetailedException::add_detail(const std::string &paramName, const char *paramValue) throw()
{
	return add_detail(paramName, std::string(paramValue));
}

DetailedException &DetailedException::add_detail(const std::string &paramName, const std::string &paramValue) throw()
{
	params.Add(paramName + "=" + paramValue);
	return *this;
}

DetailedException::DetailedException(const std::string &message) throw()
	: message(message)
{
}

DetailedException::DetailedException(const DetailedException &src) throw()
	: message(src.message), params(src.params)
{
}

const char *DetailedException::what() const throw()
{
	return message.c_str();
}


FieldStreamParseException::FieldStreamParseException(const std::string &message) throw()
		: DetailedException(message)
{
}

SerializationException::SerializationException(const std::string &message) throw()
		: DetailedException(message)
{
}

PackDirectoryDoesNotExistException::PackDirectoryDoesNotExistException(const std::string &message) throw()
		: DetailedException(message)
{
}

SupplierConfigReadException::SupplierConfigReadException(const std::string &message) throw()
	: DetailedException(message)
{
}
