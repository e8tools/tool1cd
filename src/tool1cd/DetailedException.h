//
// Created by dmpas on 07.01.18.
//

#ifndef TOOL1CD_PROJECT_DETAILEDEXCEPTION_H
#define TOOL1CD_PROJECT_DETAILEDEXCEPTION_H

#include <exception>
#include <System.Classes.hpp>

class DetailedException : public std::exception {
public:
	DetailedException(const String &message) throw();
	DetailedException(const DetailedException &src) throw();

	void show() const throw();
	DetailedException &add_detail(const String &paramName, const String &paramValue) throw();

	virtual const char *what() const throw() override ;

private:
	String message;
	TStringList params;
};


class FieldStreamParseException : public DetailedException
{
public:
	FieldStreamParseException(const String &message) throw();
};

class SerializationException : public DetailedException
{
public:
	SerializationException(const String &message) throw();
};

class PackDirectoryDoesNotExistException : public DetailedException
{
public:
		PackDirectoryDoesNotExistException(const String &message) throw();
};

#endif //TOOL1CD_PROJECT_DETAILEDEXCEPTION_H
