//---------------------------------------------------------------------------

#ifndef ErrorRegistrationH
#define ErrorRegistrationH

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
enum MessageState
{
	msEmpty = -1,
	msSuccesfull = 0,
	msWarning = 1,
	msInfo = 2,
	msError = 3,
	msWait = 4,
	msHint = 5
};


//---------------------------------------------------------------------------
class MessageRegistrator
{
private:
	bool DebugMessage;

public:
	__fastcall MessageRegistrator();
	void __fastcall setDebugMode(bool dstate);
	virtual void __fastcall AddMessage(UnicodeString message, MessageState mstate, TStringList* param = NULL) = 0;
	void __fastcall AddError(UnicodeString description);
	void __fastcall AddError(UnicodeString description,
				UnicodeString parname1, UnicodeString par1
	);
	void __fastcall AddError(UnicodeString description,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2
	);
	void __fastcall AddError(UnicodeString description,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3
	);
	void __fastcall AddError(UnicodeString description,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3,
				UnicodeString parname4, UnicodeString par4
	);
	void __fastcall AddError(UnicodeString description,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3,
				UnicodeString parname4, UnicodeString par4,
				UnicodeString parname5, UnicodeString par5
	);

	void __fastcall AddMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1
	);
	void __fastcall AddMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2
	);
	void __fastcall AddMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3
	);
	void __fastcall AddMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3,
				UnicodeString parname4, UnicodeString par4
	);
	void __fastcall AddMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3,
				UnicodeString parname4, UnicodeString par4,
				UnicodeString parname5, UnicodeString par5
	);

	void __fastcall AddDebugMessage(UnicodeString description, MessageState mstate);

	void __fastcall AddDebugMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1
	);
	void __fastcall AddDebugMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2
	);
	void __fastcall AddDebugMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3
	);
	void __fastcall AddDebugMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3,
				UnicodeString parname4, UnicodeString par4
	);

	void __fastcall AddDebugMessage(UnicodeString description, MessageState mstate,
				UnicodeString parname1, UnicodeString par1,
				UnicodeString parname2, UnicodeString par2,
				UnicodeString parname3, UnicodeString par3,
				UnicodeString parname4, UnicodeString par4,
				UnicodeString parname5, UnicodeString par5
	);
};

//---------------------------------------------------------------------------
UnicodeString tohex(int n);

//---------------------------------------------------------------------------

#endif
