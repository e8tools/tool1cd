//---------------------------------------------------------------------------

#pragma hdrstop

#include "ErrorRegistration.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
UnicodeString tohex(int n)
{
	return UnicodeString("0x") + UnicodeString::IntToHex(n, 0);
}

//---------------------------------------------------------------------------
__fastcall MessageRegistrator::MessageRegistrator()
{
	DebugMessage = false;
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::setDebugMode(bool dstate)
{
	DebugMessage = dstate;
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddError(UnicodeString description)
{
	AddMessage(description, msError);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddError(UnicodeString description,
	UnicodeString parname1, UnicodeString par1)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddError(UnicodeString description,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddError(UnicodeString description,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	ts->Add(parname3 + "=" + par3);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddError(UnicodeString description,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3,
	UnicodeString parname4, UnicodeString par4)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	ts->Add(parname3 + "=" + par3);
	ts->Add(parname4 + "=" + par4);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddError(UnicodeString description,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3,
	UnicodeString parname4, UnicodeString par4,
	UnicodeString parname5, UnicodeString par5)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	ts->Add(parname3 + "=" + par3);
	ts->Add(parname4 + "=" + par4);
	ts->Add(parname5 + "=" + par5);
	AddMessage(description, msError, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	ts->Add(parname3 + "=" + par3);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3,
	UnicodeString parname4, UnicodeString par4)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	ts->Add(parname3 + "=" + par3);
	ts->Add(parname4 + "=" + par4);
	AddMessage(description, mstate, ts);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3,
	UnicodeString parname4, UnicodeString par4,
	UnicodeString parname5, UnicodeString par5)
{
	TStringList* ts = new TStringList;
	ts->Add(parname1 + "=" + par1);
	ts->Add(parname2 + "=" + par2);
	ts->Add(parname3 + "=" + par3);
	ts->Add(parname4 + "=" + par4);
	ts->Add(parname5 + "=" + par5);
	AddMessage(description, mstate, ts);
}


//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddDebugMessage(UnicodeString description, MessageState mstate)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate);
}


//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddDebugMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate,
		parname1, par1);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddDebugMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate,
		parname1, par1,
		parname2, par2);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddDebugMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddDebugMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3,
	UnicodeString parname4, UnicodeString par4)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3,
		parname4, par4);
}

//---------------------------------------------------------------------------
void __fastcall MessageRegistrator::AddDebugMessage(UnicodeString description, MessageState mstate,
	UnicodeString parname1, UnicodeString par1,
	UnicodeString parname2, UnicodeString par2,
	UnicodeString parname3, UnicodeString par3,
	UnicodeString parname4, UnicodeString par4,
	UnicodeString parname5, UnicodeString par5)
{
	if(!DebugMessage) return;
	AddMessage(description, mstate,
		parname1, par1,
		parname2, par2,
		parname3, par3,
		parname4, par4,
		parname5, par5);
}

