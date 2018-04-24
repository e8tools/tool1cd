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

#ifndef MessageRegistrationH
#define MessageRegistrationH

#include <System.Classes.hpp>

//---------------------------------------------------------------------------
enum class MessageState
{
	Empty = -1,
	Hint = 0,
	Succesfull = 1,
	Info = 2,
	Warning = 3,
	Error = 4
};

class IMessageRegistration
{
public:
	virtual void AddDetailedMessage(const std::string &description, const MessageState mstate, const TStringList *param) = 0;
	virtual void Status(const std::string &message) = 0;
};

class MessageDetails
{
public:
	MessageDetails(IMessageRegistration &owner, const std::string &initial_message, MessageState mstate)
	        : owner(owner), initial_message(initial_message), mstate(mstate)
	{}

	MessageDetails &with(const std::string &param_name, const std::string &value)
	{
		std::string total_string(param_name);
		total_string.append("=").append(value);
		data.push_back(total_string);
		return *this;
	}

	template <typename value_type>
	MessageDetails &with(const std::string &param_name, value_type value)
	{
		return with(param_name, std::to_string(value));
	}

	~MessageDetails()
	{
		owner.AddDetailedMessage(initial_message, mstate, &data);
	}
private:
	IMessageRegistration &owner;
	std::string initial_message;
	MessageState mstate;
	TStringList data;
};

//---------------------------------------------------------------------------
class MessageRegistrator: public IMessageRegistration
{
private:
	bool DebugMessage;

public:
	MessageRegistrator();
	virtual ~MessageRegistrator();
	void setDebugMode(bool dstate);
	bool getDebugMode();

	virtual MessageDetails AddMessage(const std::string &description, const MessageState mstate);
	virtual MessageDetails AddError(const std::string &description);
	virtual MessageDetails AddDebugMessage(const std::string &description, const MessageState mstate);

};

//---------------------------------------------------------------------------

class IControlMessageRegistration
{
public:
	virtual ~IControlMessageRegistration() {}
	virtual void AddMessageRegistrator(MessageRegistrator* messageregistrator) = 0;
	virtual void RemoveMessageRegistrator() = 0;
};

class Registrator: public MessageRegistrator, public IControlMessageRegistration
{
public:
	void AddDetailedMessage(const std::string &description, const MessageState mstate,
	                        const TStringList *param) override;
	void Status(const std::string &message) override;

	void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	void RemoveMessageRegistrator() override;
private:
	MessageRegistrator* msreg_m{nullptr};
};

#endif

