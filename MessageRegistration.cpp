//---------------------------------------------------------------------------

#pragma hdrstop

#include "MessageRegistration.h"

//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

//---------------------------------------------------------------------------
MessageRegistrator::MessageRegistrator()
{
	DebugMessage = false;
}

//---------------------------------------------------------------------------
void MessageRegistrator::setDebugMode(bool dstate)
{
	DebugMessage = dstate;
}

//---------------------------------------------------------------------------
bool MessageRegistrator::getDebugMode()
{
	return DebugMessage;
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description)
{
	AddMessage(description, msError);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	ts->Add(parname5 + " = " + par5);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5,
	const String& parname6, const String& par6)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	ts->Add(parname5 + " = " + par5);
	ts->Add(parname6 + " = " + par6);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddError(const String& description,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5,
	const String& parname6, const String& par6,
	const String& parname7, const String& par7)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	ts->Add(parname5 + " = " + par5);
	ts->Add(parname6 + " = " + par6);
	ts->Add(parname7 + " = " + par7);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	ts->Add(parname5 + " = " + par5);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5,
	const String& parname6, const String& par6)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	ts->Add(parname5 + " = " + par5);
	ts->Add(parname6 + " = " + par6);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddMessage_(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5,
	const String& parname6, const String& par6,
	const String& parname7, const String& par7)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + " = " + par1);
	ts->Add(parname2 + " = " + par2);
	ts->Add(parname3 + " = " + par3);
	ts->Add(parname4 + " = " + par4);
	ts->Add(parname5 + " = " + par5);
	ts->Add(parname6 + " = " + par6);
	ts->Add(parname7 + " = " + par7);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate);
}


//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1,
		parname2, par2);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3,
		parname4, par4);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3,
		parname4, par4,
		parname5, par5);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5,
	const String& parname6, const String& par6)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3,
		parname4, par4,
		parname5, par5,
		parname6, par6);
}

//---------------------------------------------------------------------------
void MessageRegistrator::AddDebugMessage(const String& description, const MessageState mstate,
	const String& parname1, const String& par1,
	const String& parname2, const String& par2,
	const String& parname3, const String& par3,
	const String& parname4, const String& par4,
	const String& parname5, const String& par5,
	const String& parname6, const String& par6,
	const String& parname7, const String& par7)
{
	if(!DebugMessage) return;
	AddMessage_(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3,
		parname4, par4,
		parname5, par5,
		parname6, par6,
		parname7, par7);
}

