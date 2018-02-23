//
// Created by dmpas on 07.01.18.
//

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
