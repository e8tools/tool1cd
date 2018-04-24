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
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "Messenger.h"

using namespace std;

Messenger::Messenger()
{
	has_error = false;
	logfile = "";
	noverbose = false;
	minimal_state = MessageState::Empty;
}

//---------------------------------------------------------------------------
void Messenger::Status(const string &message)
{
	cout << message << endl;
	AddMessage(message, MessageState::Empty);
}

//---------------------------------------------------------------------------
void Messenger::AddDetailedMessage(const string &message, const MessageState mstate, const TStringList *param)
{
	if (mstate < minimal_state) {
		return;
	}

	shared_ptr<ostream> output (&cerr, [](...){} );

	if (!logfile.empty()) {
		output = make_shared<boost::filesystem::ofstream>(boost::filesystem::path(logfile), std::ios_base::app);
	}
	*output << message << endl;
	if (param) {
		for (auto it : *param) {
			*output << "\t" << it << endl;
		}
	}
	output->flush();
}

//---------------------------------------------------------------------------
void Messenger::setlogfile(const string &_logfile)
{
	logfile = System::Ioutils::TPath::GetFullPath(_logfile);
	if(FileExists(logfile)) DeleteFile(logfile);
}

void Messenger::set_log_level(const MessageState _minimal_state)
{
	minimal_state = _minimal_state;
}