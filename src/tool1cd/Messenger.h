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
#ifndef MESSENGER_H_INCLUDED
#define MESSENGER_H_INCLUDED

#include "MessageRegistration.h"

class Messenger : public MessageRegistrator
{
private:
	std::string logfile;
	bool noverbose;
	MessageState minimal_state;
public:
	bool has_error;
	Messenger();
	virtual void AddDetailedMessage(const std::string &message, const MessageState mstate,
									const TStringList *param = nullptr) override;
	virtual void Status(const std::string &message) override;
	void setlogfile(const std::string &_logfile);
	void setnoverbose(bool nv){noverbose = nv;}
	void set_log_level(const MessageState _minimal_state);
};

#endif // MESSENGER_H_INCLUDED
