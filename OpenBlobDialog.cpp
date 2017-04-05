//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OpenBlobDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvDirectoryEdit"
#pragma link "AdvEdBtn"
#pragma link "AdvEdit"
#pragma link "AdvFileNameEdit"
#pragma link "AdvToolBtn"
#pragma resource "*.dfm"
TFormOpenBLOB *FormOpenBLOB;
//---------------------------------------------------------------------------
__fastcall TFormOpenBLOB::TFormOpenBLOB(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormOpenBLOB::CheckBoxBlobParseContainerClick(TObject *Sender)
{
	SetFileDirectoryMode();
	SetOKEnabled();
}
//---------------------------------------------------------------------------
void __fastcall TFormOpenBLOB::SetFileDirectoryMode()
{
	LabelCaption->Caption = CheckBoxBlobParseContainer->Checked ? L"Каталог" : L"Файл";
	DirectoryEdit->Visible = CheckBoxBlobParseContainer->Checked;
	FileNameEdit->Visible = !CheckBoxBlobParseContainer->Checked;
}
void __fastcall TFormOpenBLOB::DirectoryEditChange(TObject *Sender)
{
	SetOKEnabled();
}
//---------------------------------------------------------------------------
void __fastcall TFormOpenBLOB::SetOKEnabled()
{
	if(CheckBoxBlobParseContainer->Checked)
	{
		ButtonOK->Enabled = DirectoryEdit->Text.Length() != 0;
	}
	else
	{
		ButtonOK->Enabled = FileNameEdit->Text.Length() != 0;
	}
}
void __fastcall TFormOpenBLOB::FileNameEditChange(TObject *Sender)
{
	SetOKEnabled();
}
//---------------------------------------------------------------------------
void __fastcall TFormOpenBLOB::FormShow(TObject *Sender)
{
	CheckBoxBlobParseContainer->Checked = false;
	CheckBoxBlobParseUsersUsr->Checked = false;
	CheckBoxBlobParseUnzip->Checked = false;
	CheckBoxBlobParseUnzip2->Checked = false;
	CheckBoxChangeRelatedFields->Checked = false;
	CheckBoxBlobParseValue1C->Checked = false;
	CheckBoxBlobParseValueStorage->Checked = false;

	CheckBoxBlobParseContainer->Visible = false;
	CheckBoxBlobParseUsersUsr->Visible = false;
	CheckBoxBlobParseUnzip->Visible = false;
	CheckBoxBlobParseUnzip2->Visible = false;
	CheckBoxChangeRelatedFields->Visible = false;
	CheckBoxBlobParseValue1C->Visible = false;
	CheckBoxBlobParseValueStorage->Visible = false;

	if(obMode == obmDefault)
	{

	}
	else if(obMode == obmFiles)
	{
		CheckBoxChangeRelatedFields->Visible = true;
		CheckBoxChangeRelatedFields->Checked = true;
		CheckBoxBlobParseContainer->Visible = true;
		CheckBoxBlobParseUnzip->Visible = true;
		CheckBoxBlobParseUnzip2->Visible = true;
		CheckBoxBlobParseContainer->Top = CheckBoxChangeRelatedFields->Top + 1;
		CheckBoxBlobParseUnzip->Top = CheckBoxBlobParseContainer->Top + 1;
		CheckBoxBlobParseUnzip2->Top = CheckBoxBlobParseUnzip->Top + 1;
	}
	else if(obMode == obmSystem)
	{
//		CheckBoxBlobParseContainer->Visible = true;
		CheckBoxBlobParseUnzip->Visible = true;
//		CheckBoxBlobParseUnzip2->Visible = true;
	}
	else if(obMode == obmUserUsr)
	{
		CheckBoxChangeRelatedFields->Visible = true;
		CheckBoxChangeRelatedFields->Checked = true;
		CheckBoxBlobParseUsersUsr->Visible = true;
		CheckBoxBlobParseUsersUsr->Checked = true;
		CheckBoxBlobParseUsersUsr->Top = CheckBoxChangeRelatedFields->Top + 1;
	}

	FileNameEdit->Text = L"";
	DirectoryEdit->Text = L"";

	SetFileDirectoryMode();
	SetOKEnabled();
}
//---------------------------------------------------------------------------

