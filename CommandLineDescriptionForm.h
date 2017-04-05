//---------------------------------------------------------------------------

#ifndef CommandLineDescriptionFormH
#define CommandLineDescriptionFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "HTMLText.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "AdvMemo.hpp"
//---------------------------------------------------------------------------
class TFormCommandLineDescription : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo;
private:	// User declarations
public:		// User declarations
	__fastcall TFormCommandLineDescription(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCommandLineDescription *FormCommandLineDescription;
//---------------------------------------------------------------------------
#endif
