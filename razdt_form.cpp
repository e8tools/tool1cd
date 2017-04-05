//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "razdt_form.h"
#include "zip.h"
#include "TempStream.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvEdBtn"
#pragma link "AdvEdit"
#pragma link "AdvFileNameEdit"
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

////---------------------------------------------------------------------------
//String __fastcall operator +(const wchar_t*& lhs, const String& rhs)
//{
//	String out(lhs);
//	out += rhs;
//	return out;
//}
//
//String __fastcall operator +(const wchar_t*& lhs, const wchar_t*& rhs)
//{
//	String out(lhs);
//	out += rhs;
//	return out;
//}

//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonRazDTClick(TObject *Sender)
{
	unsigned char b;
	unsigned char bi[512];
	unsigned char* bi2;
	wchar_t wb[33];
	wchar_t wb2[33];
	wchar_t* wb3;
	unsigned int i;
	unsigned int l;
	unsigned __int64 ll;
	bool f;
	bool endlevel;
	TStream* in;
	TStream* temp;
	TStream* fout;
	TStreamWriter* out;
	enum{
		f_stream,
		f_dtv0, // dt v8.0
		f_dtv1, // dt v8.1
		f_dtv2, // dt v8.2
		f_dtv3  // dt v8.3
	}format;
	bool quotes;

	if(AdvFileNameEditDT->Text.IsEmpty())
	{
		MessageBox(NULL, L"Не выбран файл", L"Ошибка", MB_ICONSTOP|MB_OK);
		return;
	}

	try
	{
		in = new TFileStream(AdvFileNameEditDT->Text, fmOpenRead);
	}
	catch(...)
	{
		MessageBox(NULL, (L"Не удалось открыть файл " + AdvFileNameEditDT->Text).c_str(), L"Ошибка", MB_ICONSTOP|MB_OK);
		return;
	}

	if(in->Read(bi, 9) == 9)
	{
		if(memcmp(bi, "1CIBDmpF", 8) == 0)
		{
			if(bi[8] == '1') format = f_dtv1;
			else if(bi[8] == '2') format = f_dtv2;
			else if(bi[8] == '3') format = f_dtv3;
			else if(bi[8] == 'l') format = f_dtv0;
			else
			{
				swprintf(wb, L"%02hX", bi[8]);
				MessageBox(NULL, (String(L"Неизвестный код ") + String((char*)bi + 8, 1) + L" (0x" + wb + L") формата DT").c_str(), L"Ошибка", MB_ICONSTOP|MB_OK);
				delete in;
				return;
			}
		}
		else format = f_stream;
	}

	try
	{
		fout = new TFileStream(AdvFileNameEditDT->Text + L".rdt", fmCreate);
	}
	catch(...)
	{
		MessageBox(NULL, (L"Не удалось создать файл " + AdvFileNameEditDT->Text + L".rdt").c_str(), L"Ошибка", MB_ICONSTOP|MB_OK);
		return;
	}

	switch(format)
	{
		case f_stream:
			in->Seek(0l, soBeginning);
			break;
		case f_dtv0:
		case f_dtv1:
			InflateStream(in, fout);
			delete in;
			delete fout;
			return;
		case f_dtv2:
		case f_dtv3:
			temp = new TTempStream;
			InflateStream(in, temp);
			delete in;
			in = temp;
			in->Seek(0l, soBeginning);
			break;
	}

	fout->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
	out = new TStreamWriter(fout, TEncoding::UTF8, 4096);
	out->AutoFlush = false;
	endlevel = false;
	quotes = true;

	while(in->Read(&b, 1))
	{
		if(b & 0x20)
		{
			if(endlevel) out->WriteLine();
			out->Write(String(L"}"));
			endlevel = true;
		}
		if(b & 0x80)
		{
			out->Write(String(L","));
			endlevel = false;
		}
		if(b & 0x40)
		{
			out->WriteLine();
			out->Write(String(L"{"));
			endlevel = false;
		}
		b &= 0x1f;
		switch(b)
		{
			case 0:
				break;
			case 1:
				out->Write(String(L"0"));
				break;
			case 2:
				out->Write(String(L"1"));
				break;
			case 3:
				out->Write(String(L"2"));
				break;
			case 4:
				out->Write(String(L"3"));
				break;
			case 5:
				out->Write(String(L"4"));
				break;
			case 6:
				out->Write(String(L"5"));
				break;
			case 7:
				out->Write(String(L"6"));
				break;
			case 8:
				out->Write(String(L"7"));
				break;
			case 9:
				out->Write(String(L"8"));
				break;
			case 0xa:
				out->Write(String(L"9"));
				break;
			case 0xb:
				in->Read(&bi, 1);
				out->Write(String(*bi));
				break;
			case 0xc:
				in->Read(&bi, 1);
				out->Write(String(L"-"));
				out->Write(String(*bi));
				break;
			case 0xd:
				in->Read(&bi, 2);
				out->Write(String(*(unsigned short*)bi));
				break;
			case 0xe:
				in->Read(&bi, 2);
				out->Write(String(L"-"));
				out->Write(String(*(unsigned short*)bi));
				break;
			case 0xf:
				in->Read(&bi, 4);
				out->Write(String(*(unsigned int*)bi));
				break;
			case 0x10:
				in->Read(&bi, 4);
				out->Write(String(L"-"));
				out->Write(String(*(unsigned int*)bi));
				break;
			case 0x11:
				in->Read(&bi, 8);
				out->Write(String(*(unsigned __int64*)bi));
				break;
			case 0x15:
				in->Read(&bi, 16);
				swprintf(wb, L"%08x", *(unsigned int*)bi);
				out->Write(String(wb, 8));
				out->Write(String(L"-"));
				for(i = 4; i <= 6; i += 2)
				{
					swprintf(wb, L"%04hx", *(unsigned short*)(bi + i));
					out->Write(String(wb, 4));
					out->Write(String(L"-"));
				}
				for(i = 8; i < 10; ++i)
				{
					swprintf(wb, L"%02hx", bi[i]);
					out->Write(String(wb, 2));
				}
				out->Write(String(L"-"));
				for(i = 10; i < 16; ++i)
				{
					swprintf(wb, L"%02hx", bi[i]);
					out->Write(String(wb, 2));
				}
				break;
			case 0x16:
				quotes = false;
				break;
			case 0x17:
				l = 0;
				in->Read(&l, 1);
				in->Read(&bi, l * 2);
				if(quotes) out->Write(String(L"\""));
				out->Write(String((wchar_t*)bi, l));
				if(quotes) out->Write(String(L"\""));
				quotes = true;
				break;
			case 0x18:
				l = 0;
				in->Read(&l, 2);
				wb3 = new wchar_t[l];
				in->Read(wb3, l * 2);
				if(quotes) out->Write(String(L"\""));
				out->Write(String(wb3, l));
				if(quotes) out->Write(String(L"\""));
				delete[] wb3;
				quotes = true;
				break;
			case 0x1a:
				l = 0;
				in->Read(&l, 1);
				in->Read(&bi, l);
				f = true;
				for(i = 0; i < l; ++i)
				{
					if(bi[i] < 0x20 || bi[i] > 0x7f)
					{
						f = false;
						break;
					}
				}
				if(f)
				{
					if(quotes) out->Write(String(L"\""));
					out->Write(String((char *)bi, l));
					if(quotes) out->Write(String(L"\""));
				}
				else
				{
					for(i = 0; i < l; ++i)
					{
						swprintf(wb, L"%02hX", bi[i]);
						out->Write(String(wb, 2));
					}
				}
				quotes = true;
				break;
			case 0x1b:
				l = 0;
				in->Read(&l, 2);
				bi2 = new unsigned char[l];
				in->Read(bi2, l);
				f = true;
				for(i = 0; i < l; ++i)
				{
					if(bi2[i] < 0x20 || bi2[i] > 0x7f)
					{
						f = false;
						break;
					}
				}
				if(f)
				{
					if(quotes) out->Write(String(L"\""));
					out->Write(String((char *)bi2, l));
					if(quotes) out->Write(String(L"\""));
				}
				else
				{
					for(i = 0; i < l; ++i)
					{
						swprintf(wb, L"%02hX", bi2[i]);
						out->Write(String(wb, 2));
					}
				}
				delete[] bi2;
				quotes = true;
				break;
			case 0x1c:
				in->Read(&ll, 8);
				bi2 = new unsigned char[ll];
				in->Read(bi2, ll);
				f = true;
				for(i = 0; i < ll; ++i)
				{
					if(bi2[i] < 0x20 || bi2[i] > 0x7f)
					{
						f = false;
						break;
					}
				}
				if(f)
				{
					if(quotes) out->Write(String(L"\""));
					out->Write(String((char *)bi2, ll));
					if(quotes) out->Write(String(L"\""));
				}
				else
				{
					for(i = 0; i < ll; ++i)
					{
						swprintf(wb, L"%02hX", bi2[i]);
						out->Write(String(wb, 2));
					}
				}
				delete[] bi2;
				quotes = true;
				break;
			default:
				swprintf(wb, L"%02hX", b);
				swprintf(wb2, L"%lX", in->Position - 1);
				MessageBox(NULL, (String(L"Неизвестный код 0x") + wb + L" по адресу 0x" + wb2).c_str(), L"Ошибка", MB_ICONSTOP|MB_OK);
				in->Seek(0l, soEnd);
				break;
		}
	}

	delete in;
	out->Flush();
	delete out;
	delete fout;
}
//---------------------------------------------------------------------------

