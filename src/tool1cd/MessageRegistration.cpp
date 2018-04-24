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

#include "MessageRegistration.h"

using namespace std;

MessageDetails MessageRegistrator::AddMessage(const string &description, const MessageState mstate)
{
	MessageDetails details(*this, static_cast<string>(description), mstate);
	return details;
}

MessageDetails MessageRegistrator::AddError(const string &description)
{
	return AddMessage(description, MessageState::Error);
}

MessageDetails MessageRegistrator::AddDebugMessage(const string &description, const MessageState mstate)
{
	// TODO: AddDebugMessage
	return AddMessage(description, mstate);
}

MessageRegistrator::MessageRegistrator()
{

}

MessageRegistrator::~MessageRegistrator()
{

}

void MessageRegistrator::setDebugMode(bool dstate)
{
	DebugMessage = dstate;
}

bool MessageRegistrator::getDebugMode()
{
	return DebugMessage;
}


void Registrator::AddMessageRegistrator(MessageRegistrator* messageregistrator)
{
	msreg_m = messageregistrator;
}

void Registrator::RemoveMessageRegistrator()
{
	msreg_m = nullptr;
}

void Registrator::Status(const string &message)
{

}

void Registrator::AddDetailedMessage(const string &description, const MessageState mstate,
                                     const TStringList *param)
{
	if (msreg_m) {
		msreg_m->AddDetailedMessage(description, mstate, param);
	}
}