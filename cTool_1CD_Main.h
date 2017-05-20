//---------------------------------------------------------------------------

#ifndef CTool_1CD_MainH
#define CTool_1CD_MainH
//---------------------------------------------------------------------------
#include <iostream>
//#include <vcl.h>
//#include <windows.h>

#pragma hdrstop
#pragma argsused

//#include <tchar.h>
#include <stdio.h>
//#include <windows.h>
#include "Class_1CD.h"
#include "MessageRegistration.h"
#include "Parse_tree.h"

//---------------------------------------------------------------------------
class Messager : public MessageRegistrator
{
private:
	String logfile;
	bool noverbose;
public:
	bool has_error;
	Messager();
	virtual void AddMessage(const String& message, const MessageState mstate, TStringList* param = NULL);
	virtual void Status(const String& message);
	void setlogfile(String _logfile);
	void setnoverbose(bool nv){noverbose = nv;};
};

#endif

