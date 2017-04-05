//---------------------------------------------------------------------------

#ifndef MessageRegistrationH
#define MessageRegistrationH

#include <System.Classes.hpp>
//#include <vcl.h>
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
	bool __fastcall getDebugMode();
	virtual void __fastcall AddMessage(const String& description, const MessageState mstate, TStringList* param = NULL) = 0;
	virtual void __fastcall Status(const String& message) = 0;
	void __fastcall AddError(const String& description);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1
	);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void __fastcall AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);

	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	);
	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);
	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void __fastcall AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);

	void __fastcall AddDebugMessage(const String& description, const MessageState mstate);

	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	);
	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);

	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void __fastcall AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);
};

//---------------------------------------------------------------------------

#endif

