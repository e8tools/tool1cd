//---------------------------------------------------------------------------

#ifndef OpenBlobDialogH
#define OpenBlobDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvDirectoryEdit.hpp"
#include "AdvEdBtn.hpp"
#include "AdvEdit.hpp"
#include "AdvFileNameEdit.hpp"
#include "AdvToolBtn.hpp"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
enum OpenBlobMode
{
	obmDefault,
	obmFiles,
	obmSystem,
	obmUserUsr
};
//---------------------------------------------------------------------------
class TFormOpenBLOB : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBoxBlobParseMode;
	TCheckBox *CheckBoxBlobParseValueStorage;
	TCheckBox *CheckBoxBlobParseValue1C;
	TCheckBox *CheckBoxBlobParseUnzip;
	TCheckBox *CheckBoxBlobParseUnzip2;
	TCheckBox *CheckBoxChangeRelatedFields;
	TCheckBox *CheckBoxBlobParseContainer;
	TAdvDirectoryEdit *DirectoryEdit;
	TAdvFileNameEdit *FileNameEdit;
	TPanel *Panel1;
	TLabel *LabelCaption;
	TButton *ButtonOK;
	TButton *Button1;
	TCheckBox *CheckBoxBlobParseUsersUsr;
	void __fastcall CheckBoxBlobParseContainerClick(TObject *Sender);
	void __fastcall DirectoryEditChange(TObject *Sender);
	void __fastcall FileNameEditChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	void __fastcall SetFileDirectoryMode();
	void __fastcall SetOKEnabled();
public:		// User declarations
	OpenBlobMode obMode;
	__fastcall TFormOpenBLOB(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOpenBLOB *FormOpenBLOB;
//---------------------------------------------------------------------------
#endif
