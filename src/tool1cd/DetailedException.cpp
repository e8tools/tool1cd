//
// Created by dmpas on 07.01.18.
//

#include "DetailedException.h"
#include "MessageRegistration.h"

extern Registrator msreg_g;

void DetailedException::show() const throw()
{
	msreg_g.AddDetailedMessage(message, MessageState::Error, &params);
}

DetailedException &DetailedException::add_detail(const String &paramName, const String &paramValue) throw()
{
	params.Add(paramName + "=" + paramValue);
	return *this;
}

DetailedException::DetailedException(const String &message) throw()
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


FieldStreamParseException::FieldStreamParseException(const String &message) throw()
		: DetailedException(message)
{
}

SerializationException::SerializationException(const String &message) throw()
		: DetailedException(message)
{
}

PackDirectoryDoesNotExistException::PackDirectoryDoesNotExistException(const System::String &message) throw()
		: DetailedException(message)
{
}

SupplierConfigReadException::SupplierConfigReadException(const System::String &message) throw()
	: DetailedException(message)
{
}
