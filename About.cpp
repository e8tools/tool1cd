//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;

bool image_eye;
int count_eye;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner) : TForm(AOwner)
{
//	TCanvas* c;
//	TRect r(0, 0, ImageCopyLeft->Width, ImageCopyLeft->Height);
//
//	c = ImageCopyLeft->Canvas;
//
//	c->Brush->Color = clBtnFace;
//	c->Brush->Style = bsSolid;
//
//	c->FillRect(r);
//
//	c->Font->Charset = Copyright->Font->Charset;
//	c->Font->Color = Copyright->Font->Color;
////	c->Font->Height = Copyright->Font->Height;
//	c->Font->Name = Copyright->Font->Name;
//	c->Font->Orientation = 1800;
//	c->Font->Pitch = Copyright->Font->Pitch;
//	c->Font->Size = Copyright->Font->Size + 1;
//	c->Font->Style = Copyright->Font->Style;
//
//	c->TextOutA(ImageCopyLeft->Width, ImageCopyLeft->Height, L"©");
}
//---------------------------------------------------------------------


void __fastcall TAboutBox::ProgramIconClick(TObject *Sender)
{
	ShellExecute(0, 0, L"http://infostart.ru/public/19633/", 0, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::Timer1Timer(TObject *Sender)
{
	if(image_eye)
	{
		image_eye = false;
		ProgramIcon->Picture = Image1->Picture;
		//ProgramIcon->Invalidate();
	}
	else
	{
		if(++count_eye >= 6)
		{
			image_eye = true;
			count_eye = 0;
			ProgramIcon->Picture = Image2->Picture;
			//ProgramIcon->Invalidate();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	//ProgramIcon->Picture = Image1->Picture;
	image_eye = false;
	count_eye = 0;
	//Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;
}
//---------------------------------------------------------------------------










