//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CommandLineDescriptionForm.h"
#include "ParseCommandLine.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTMLText"
#pragma link "AdvMemo"
#pragma resource "*.dfm"
TFormCommandLineDescription *FormCommandLineDescription;
//---------------------------------------------------------------------------
__fastcall TFormCommandLineDescription::TFormCommandLineDescription(TComponent* Owner) : TForm(Owner)
{
	Memo->Lines->Text = CommandParse::gethelpstring().c_str();
}
//---------------------------------------------------------------------------
