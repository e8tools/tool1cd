//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("OpenBlobDialog.cpp", FormOpenBLOB);
USEFORM("Main_1CD.cpp", FormMain);
USEFORM("CommandLineDescriptionForm.cpp", FormCommandLineDescription);
USEFORM("About.cpp", AboutBox);
//---------------------------------------------------------------------------
int ret_code = 0;
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "Tool_1CD";
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFormOpenBLOB), &FormOpenBLOB);
		Application->CreateForm(__classid(TFormCommandLineDescription), &FormCommandLineDescription);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return ret_code;
}
//---------------------------------------------------------------------------


