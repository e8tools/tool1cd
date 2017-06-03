
#ifndef MessageRegistrationH
#define MessageRegistrationH

#include <System.Classes.hpp>
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
	MessageRegistrator();
	virtual ~MessageRegistrator();
	void setDebugMode(bool dstate);
	bool getDebugMode();
	virtual void AddMessage(const String& description, const MessageState mstate, TStringList* param = NULL) = 0;
	virtual void Status(const String& message) = 0;
	void AddError(const String& description);
	void AddError(const String& description,
				const String& parname1, const String& par1
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);

	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);

	void AddDebugMessage(const String& description, const MessageState mstate);

	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);

	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
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

