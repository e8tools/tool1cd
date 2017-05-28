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

}

//---------------------------------------------------------------------------
void Messenger::Status(const String& message)
{
	cout << message << endl;
	AddMessage(message, msEmpty, nullptr);
}

//---------------------------------------------------------------------------
void Messenger::AddMessage(const String& message, const MessageState mstate, TStringList* param)
{
	shared_ptr<ostream> output (&cerr, [](...){} );

	if (!logfile.IsEmpty()) {
		output = make_shared<boost::filesystem::ofstream>(boost::filesystem::path(static_cast<string>(logfile)), std::ios_base::app);
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
void Messenger::setlogfile(String _logfile)
{
	logfile = System::Ioutils::TPath::GetFullPath(_logfile);
	if(FileExists(logfile)) DeleteFile(logfile);
}
