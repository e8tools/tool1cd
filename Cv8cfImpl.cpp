// CV8CFIMPL : Implementation of TCv8cf2Impl (CoClass: Cv8cf, Interface: ICv8cf)

//#include <vcl.h>
#include <system.hpp>
//#pragma hdrstop

#include "CV8CFIMPL.H"
#include "AbZipTyp.hpp"
#include "AbUnzPrc.hpp"

/////////////////////////////////////////////////////////////////////////////
// TCv8cfImpl

#define wbuf_len 128
wchar_t wbuf[wbuf_len]; // вспомогательный буфер для Unicode строк
char utf8_mark[] = {0xef, 0xbb, 0xbf};
char mxl810_mark[] = {'M', 'O', 'X', 'C', 'E', 'L', 0, 8, 0, 1, 0, 0, 0, 0xef, 0xbb, 0xbf};
char mxl811_mark[] = {'M', 'O', 'X', 'C', 'E', 'L', 0, 8, 0, 1, 0, 1, 0, 0xef, 0xbb, 0xbf};
char bin21_mark[] = {2, 1, 0x53, 0x4b, 0x6f, 0xf4, 0x88, 0x8d, 0xc1, 0x4e, 0xa0, 0xd5, 0xeb, 0xb6, 0xbd, 0xa0, 0xa7, 0x0d};

// Helper для TAbZipArchive
class THelper : public TObject {
public:
	void __fastcall UnzipProc(TObject* Sender, TAbArchiveItem* Item, String NewName);
};

void __fastcall THelper::UnzipProc(TObject* Sender, TAbArchiveItem* Item, String NewName)
{
	AbUnzip(Sender, (TAbZipItem*)Item, NewName);
}


STDMETHODIMP TCv8cf2Impl::Done()
{
	//MessageBox(NULL, "Done", "Ура", MB_OK);
	return S_OK;
};


STDMETHODIMP TCv8cf2Impl::GetInfo(LPSAFEARRAY* pInfo)
{
	//MessageBox(NULL, "GetInfo", "Ура", MB_OK);
	Variant* v = (Variant*)((**pInfo).pvData);
	*v = 2000;
	return S_OK;

}

STDMETHODIMP TCv8cf2Impl::Init(LPDISPATCH pConnection)
{
	//MessageBox(NULL, "Init", "Ура", MB_OK);

	return S_OK;
}

// Зарегистрировать имя расширения языка
STDMETHODIMP TCv8cf2Impl::RegisterExtensionAs(BSTR* pExtensionName)
{
	//MessageBox(NULL, "RegisterExtensionAs", "Ура", MB_OK);
	*pExtensionName = SysAllocString(L"Cv8cf2");
	return S_OK;
}


STDMETHODIMP TCv8cf2Impl::CallAsFunc(long lMethodNum, VARIANT* pvarRetValue, LPSAFEARRAY* paParams)
{
	//MessageBox(NULL, "CallAsFunc", "Ура", MB_OK);
	Variant p[3];
	int i,j;
	AnsiString s, ss, sss;
	v8catalog* c;
	v8file* f;
	int bounds[2];
	char* buf;
	TFileStream* ffrom;
	TFileStream* fto;
	TMemoryStream* ms1;
	TMemoryStream* ms2;
	TypeOfFiles tof;
	WideString w;
	__int64 i64;
	TAbZipArchive* zip;
	THelper* Helper;
	bool flag;

	Variant* ret = (Variant*)pvarRetValue;

	if(lMethodNum >= nMet) return S_FALSE;

	// проверка и распаковка параметров
	method* met = &methods[lMethodNum];
	int nPar = met->nParam;
	if(paParams == NULL)
	{
		if(nPar)
		{
			MessageBox(NULL, "Отсутствуют параметры при вызове метода", methods[lMethodNum].name.c_str(), MB_OK|MB_ICONSTOP);
			return E_FAIL;
		}
	}
	else for(i = 0; i < nPar; i++)
	{
		param* par = &(met->params[i]);
		p[i] = ((Variant*)((**paParams).pvData))[i];

		if(p[i].Type() != par->type) //if(!(par->hasDefValue && par->DefValue == p[i]))
		{
			MessageBox(NULL, "Несоотвестие типа параметра при вызове метода", methods[lMethodNum].name.c_str(), MB_OK|MB_ICONSTOP);
			return E_FAIL;
		}
	}

	switch(lMethodNum){
		case 0: // Открыть
			if(v8cat) delete v8cat;
			name = p[0];
			flag = p[1];
			v8cat = new v8catalog(name, flag);
			if(v8cat->isOpen()) *ret = true;
			else
			{
				*ret = false;
				delete v8cat;
				v8cat = NULL;
			}
			break;
		case 1: // Закрыть
			if(v8cat)
			{
				delete v8cat;
				v8cat = NULL;
			}
			break;
		case 2: // СуществуетФайл
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						*ret = false;
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						*ret = false;
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f) *ret = false;
				else *ret = !f->IsCatalog();
			}
			else ret->Clear();
			break;
		case 3: // СуществуетКаталог
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						*ret = false;
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						*ret = false;
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f) *ret = false;
				else *ret = f->IsCatalog();
			}
			else ret->Clear();
			break;
		case 4: // ПолучитьСписокФайлов
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while(s.Length() > 0)
				//while((i = s.Pos("\\")) > 0)
				{
					if((i = s.Pos("\\")) > 0)
					{
						ss = s.SubString(1, i - 1);
						s = s.SubString(i + 1, s.Length());
					}
					else
					{
						ss = s;
						s = "";
					}
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
				}
				f = c->GetFirst();
				i = 0;
				while(f)
				{
					i++;
					f = f->GetNext();
				}
				if(!i)
				{
					ret->Clear();
					return S_OK;
				}
				bounds[0] = 0;
				bounds[1] = i - 1;
				*ret = VarArrayCreate(bounds, 1, varVariant);
				f = c->GetFirst();
				i = 0;
				while(f)
				{
					ret->PutElement(f->GetFileName(), i++);
					f = f->GetNext();
				}
			}
			else ret->Clear();
			break;
		case 5: // ПолучитьСписокТолькоФайлов
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while(s.Length() > 0)
				//while((i = s.Pos("\\")) > 0)
				{
					if((i = s.Pos("\\")) > 0)
					{
						ss = s.SubString(1, i - 1);
						s = s.SubString(i + 1, s.Length());
					}
					else
					{
						ss = s;
						s = "";
					}
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
				}
				f = c->GetFirst();
				i = 0;
				while(f)
				{
					if(!f->IsCatalog()) i++;
					f = f->GetNext();
				}
				if(!i)
				{
					ret->Clear();
					return S_OK;
				}
				bounds[0] = 0;
				bounds[1] = i - 1;
				*ret = VarArrayCreate(bounds, 1, varVariant);
				f = c->GetFirst();
				i = 0;
				while(f)
				{
					if(!f->IsCatalog()) ret->PutElement(f->GetFileName(), i++);
					f = f->GetNext();
				}
			}
			else ret->Clear();
			break;
		case 6: // ПолучитьСписокТолькоКаталогов
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while(s.Length() > 0)
				//while((i = s.Pos("\\")) > 0)
				{
					if((i = s.Pos("\\")) > 0)
					{
						ss = s.SubString(1, i - 1);
						s = s.SubString(i + 1, s.Length());
					}
					else
					{
						ss = s;
						s = "";
					}
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
				}
				f = c->GetFirst();
				i = 0;
				while(f)
				{
					if(f->IsCatalog()) i++;
					f = f->GetNext();
				}
				if(!i)
				{
					ret->Clear();
					return S_OK;
				}
				bounds[0] = 0;
				bounds[1] = i - 1;
				*ret = VarArrayCreate(bounds, 1, varVariant);
				f = c->GetFirst();
				i = 0;
				while(f)
				{
					if(f->IsCatalog()) ret->PutElement(f->GetFileName(), i++);
					f = f->GetNext();
				}
			}
			else ret->Clear();
			break;
		case 7: // ЭтоКаталог
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f) ret->Clear();
				else *ret = f->IsCatalog();
			}
			else ret->Clear();
			break;
		case 8: // Прочитать
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					ret->Clear();
					return S_OK;
				}
				if(f->IsCatalog())
				{
					ret->Clear();
					return S_OK;
				}
				i = f->GetFileLength();
				if(i < 3)
				{
					ret->Clear();
					return S_OK;
				}
				buf = new char[i + 1];
				f->Read(buf, 0, i);
				if(buf[0] != -17 /*EF*/ || buf[1] != -69 /*BB*/ || buf[2] != -65 /*BF*/)
				{
					ret->Clear();
					return S_OK;
				}
				buf[i] = 0;
				s = &buf[3];
				*ret = UTF8Decode(s);
				delete[] buf;
			}
			else ret->Clear();
			break;
		case 9: // Записать
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f) c = c->CreateCatalog(ss);
					else c = f->GetCatalog();
					if(!c)
					{
						*ret = false;
						//ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					f = c->createFile(s);
				}
				else if(f->IsCatalog())
				{
					ret->Clear();
					return S_OK;
				}
				w = (wchar_t*)p[1];
				s = UTF8Encode(w);

				i = s.Length();
				buf = new char[i + 3];
				buf[0] = -17; /*EF*/
				buf[1] = -69; /*BB*/
				buf[2] = -65; /*BF*/
				strncpy(&buf[3], s.c_str(), i);
				f->Write(buf, i + 3);
				delete[] buf;
				*ret = true;
			}
			else ret->Clear();
			break;
		case 10: // Удалить
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						*ret = false;
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						*ret = false;
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					*ret = false;
					return S_OK;
				}
				f->DeleteFile();
				*ret = true;
			}
			else ret->Clear();
			break;
		case 11: // Распаковать
			s = p[0];
			ss = p[1];
			ffrom = new TFileStream(s, fmOpenRead);
			fto = new TFileStream(ss, fmCreate);
			try
			{
				InflateStream(ffrom, fto);
			}
			catch(...)
			{
				ret->Clear();
				delete ffrom;
				delete fto;
				return S_OK;
			}
			*ret = true;
			delete ffrom;
			delete fto;
			break;
		case 12: // Упаковать
			s = p[0];
			ss = p[1];
			ffrom = new TFileStream(s, fmOpenRead);
			fto = new TFileStream(ss, fmCreate);
			try
			{
				DeflateStream(ffrom, fto);
			}
			catch(...)
			{
				ret->Clear();
				delete ffrom;
				delete fto;
				return S_OK;
			}
			*ret = true;
			delete ffrom;
			delete fto;
			break;
		case 13: // ТипФайла
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					ret->Clear();
					return S_OK;
				}
				if(f->IsCatalog())
				{
					ret->Clear();
					return S_OK;
				}
				i = f->GetFileLength();
				if(i < 3)
				{
					*ret = (int)tof_unknown;
					return S_OK;
				}
				i = i < 18 ? i : 18;
				buf = new char[18];
				f->Read(buf, 0, i);
				if(!memcmp(buf, utf8_mark, 3)) *ret = (int)tof_utf8;
				else if(!memcmp(buf, mxl810_mark, 16) && i >= 16) *ret = (int)tof_mxl810;
				else if(!memcmp(buf, mxl811_mark, 16) && i >= 16) *ret = (int)tof_mxl811;
				else if(!memcmp(buf, bin21_mark, 18) && i >= 18) *ret = (int)tof_bin21;
				else *ret = (int)tof_unknown;
				delete[] buf;
				return S_OK;
			}
			else ret->Clear();
			break;
		case 14: // ТипВнешнегоФайла
			s = p[0];
			ffrom = new TFileStream(s, fmOpenRead);
			i = ffrom->Size;
			if(i < 3)
			{
				*ret = (int)tof_unknown;
				delete ffrom;
				return S_OK;
			}
			i = i < 18 ? i : 18;
			buf = new char[18];
			ffrom->Read(buf, i);
			delete ffrom;
			if(!memcmp(buf, utf8_mark, 3)) *ret = (int)tof_utf8;
			else if(!memcmp(buf, mxl810_mark, 16) && i >= 16) *ret = (int)tof_mxl810;
			else if(!memcmp(buf, mxl811_mark, 16) && i >= 16) *ret = (int)tof_mxl811;
			else if(!memcmp(buf, bin21_mark, 18) && i >= 18) *ret = (int)tof_bin21;
			else *ret = (int)tof_unknown;
			delete[] buf;
			break;
		case 15: // ПрочитатьКакТип
			i = p[1];
			tof = (TypeOfFiles)i;
			if(tof == tof_unknown)
			{
				ret->Clear();
				return S_OK;
			}
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					ret->Clear();
					return S_OK;
				}
				if(f->IsCatalog())
				{
					ret->Clear();
					return S_OK;
				}
				i = f->GetFileLength();
				if(
					(i < 3 && tof == tof_utf8)
					|| (i < 16 && (tof == tof_mxl810 || tof == tof_mxl811))
					|| (i < 18 && tof == tof_bin21)
				)
				{
					ret->Clear();
					return S_OK;
				}
				buf = new char[i + 1];
				f->Read(buf, 0, i);
				if(tof == tof_bin21)
				{
					ms1 = new TMemoryStream;
					ms2 = new TMemoryStream;
					ms1->Read(&buf[18], i - 18);
					ms1->Seek(0, soFromBeginning);
					InflateStream(ms1, ms2);
					delete ms1;
					delete[] buf;
					i = ms2->Size;
					buf = new char[i + 1];
					ms2->Seek(0, soFromBeginning);
					ms2->Write(buf, i);
					delete ms2;
					j = 11;
				}
				else if(tof == tof_utf8) j = 3;
				else if(tof == tof_mxl810) j = 16;
				else if(tof == tof_mxl811) j = 16;
				else j = 0;

				buf[i] = 0;
				s = &buf[j];
				*ret = UTF8Decode(s);
				delete[] buf;
			}
			else ret->Clear();
			break;
		case 16: // ПрочитатьВнешнийКакТип
			i = p[1];
			tof = (TypeOfFiles)i;
			if(tof == tof_unknown)
			{
				ret->Clear();
				return S_OK;
			}
			s = p[0];
			ffrom = new TFileStream(s, fmOpenRead);
			i = ffrom->Size;
			if(
				(i < 3 && tof == tof_utf8)
				|| (i < 16 && (tof == tof_mxl810 || tof == tof_mxl811))
				|| (i < 18 && tof == tof_bin21)
			)
			{
				ret->Clear();
				delete ffrom;
				return S_OK;
			}

			if(tof == tof_bin21)
			{
				ms2 = new TMemoryStream;
				ffrom->Seek(18, soFromBeginning);
				InflateStream(ffrom, ms2);
				i = ms2->Size;
				buf = new char[i + 1];
				ms2->Seek(0, soFromBeginning);
				ms2->Read(buf, i);
				delete ms2;
				j = 11;
			}
			else
			{
				buf = new char[i + 1];
				ffrom->Read(buf, i);
				if(tof == tof_utf8) j = 3;
				else if(tof == tof_mxl810) j = 16;
				else if(tof == tof_mxl811) j = 16;
				else j = 0;
			}

			delete ffrom;
			buf[i] = 0;
			s = &buf[j];
			*ret = UTF8Decode(s);
			delete[] buf;
			break;
		case 17: // ЗаписатьКакТип
			i = p[2];
			tof = (TypeOfFiles)i;
			if(tof == tof_unknown)
			{
				ret->Clear();
				return S_OK;
			}
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					f = c->createFile(s);
				}
				else if(f->IsCatalog())
				{
					ret->Clear();
					return S_OK;
				}
				w = (wchar_t*)p[1];
				s = UTF8Encode(w);

				i = s.Length();

				if(tof == tof_bin21)
				{
					ms1 = new TMemoryStream;
					ms2 = new TMemoryStream;
					i64 = i + 3;
					ms1->Write(&i64, 8);
					ms1->Write(utf8_mark, 3);
					ms1->Write(s.c_str(), i);
					ms1->Seek(0, soFromBeginning);
					DeflateStream(ms1, ms2);
					delete ms1;
					i = ms2->Size;
					j = 18;
					buf = new char[i + 18];
					memcpy(buf, bin21_mark, 18);
					ms2->Seek(0, soFromBeginning);
					ms2->Read(&buf[j], 18);
					delete ms2;
				}
				else
				{
					if(tof == tof_utf8) j = 3;
					else if(tof == tof_mxl810) j = 16;
					else if(tof == tof_mxl811) j = 16;
					else j = 0;


					buf = new char[i + j];
					if(tof == tof_utf8) memcpy(buf, utf8_mark, 3);
					else if(tof == tof_mxl810) memcpy(buf, mxl810_mark, 16);
					else if(tof == tof_mxl811) memcpy(buf, mxl811_mark, 16);
					strncpy(&buf[j], s.c_str(), i);
				}

				f->Write(buf, i + j);
				delete[] buf;
				*ret = true;
			}
			else ret->Clear();
			break;
		case 18: // ЗаписатьВнешнийКакТип
			i = p[2];
			tof = (TypeOfFiles)i;
			if(tof == tof_unknown)
			{
				ret->Clear();
				return S_OK;
			}

			s = p[0];
			fto = new TFileStream(s, fmCreate);

			w = (wchar_t*)p[1];
			s = UTF8Encode(w);

			i = s.Length();
			if(tof == tof_bin21)
			{
				fto->Write(bin21_mark, 18);
				ms1 = new TMemoryStream;
				i64 = i + 3;
				ms1->Write(&i64, 8);
				ms1->Write(utf8_mark, 3);
				ms1->Write(s.c_str(), i);
				ms1->Seek(0, soFromBeginning);
				DeflateStream(ms1, fto);
				delete ms1;
			}
			else
			{
				if(tof == tof_utf8) fto->Write(utf8_mark, 3);
				else if(tof == tof_mxl810) fto->Write(mxl810_mark, 16);
				else if(tof == tof_mxl811) fto->Write(mxl811_mark, 16);
				fto->Write(s.c_str(), i);
			}

			delete fto;
			*ret = true;
			break;
		case 19: // ПрочитатьВоВнешнийФайл
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f)
					{
						*ret = false;
						//ret->Clear();
						return S_OK;
					}
					c = f->GetCatalog();
					if(!c)
					{
						*ret = false;
						//ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					*ret = false;
					//ret->Clear();
					return S_OK;
				}

				s = p[1];
				fto = NULL;
				try
				{
					fto = new TFileStream(s, fmCreate);
					f->SaveToStream(fto);
					*ret = true;
				}
				catch(...)
				{
					*ret = false;
				}
				delete fto;
				fto = NULL;
			}
			else ret->Clear();
			break;
		case 20: // ЗаписатьИзВнешнегоФайла
			if(v8cat)
			{
				s = p[0];
				c = v8cat;
				while((i = s.Pos("\\")) > 0)
				{
					ss = s.SubString(1, i - 1);
					f = c->GetFile(ss);
					if(!f) c = c->CreateCatalog(ss);
					else c = f->GetCatalog();
					if(!c)
					{
						*ret = false;
						//ret->Clear();
						return S_OK;
					}
					s = s.SubString(i + 1, s.Length());
				}
				f = c->GetFile(s);
				if(!f)
				{
					f = c->createFile(s);
				}
				else if(f->IsCatalog())
				{
					*ret = false;
					//ret->Clear();
					return S_OK;
				}
				s = p[1];
				ffrom = NULL;
				try
				{
					ffrom = new TFileStream(s, fmOpenRead);
					f->Write(ffrom);
					*ret = true;
				}
				catch(...)
				{
					*ret = false;
				}
				delete ffrom;
				ffrom = NULL;
			}
			else ret->Clear();
			break;
		case 21: // РаспаковатьВнешнийЗипФайл
			s = p[0];
			ss = p[1];
			zip = NULL;
			Helper = NULL;
			try
			{
				zip = new TAbZipArchive(s, fmOpenRead);
				THelper* Helper = new THelper;
				zip->ExtractHelper = Helper->UnzipProc;
				zip->Load();
				CreateDirectory(ss.c_str(), NULL);
				zip->BaseDirectory = ss;
				zip->ExtractOptions << eoCreateDirs << eoRestorePath;
				zip->ExtractFiles("*.*");
				delete Helper;
				delete zip;
			}
			catch(...)
			{
				delete Helper;
				delete zip;
				ret->Clear();
				return S_OK;
			}
			*ret = true;
			break;
		case 22: // РаспаковатьФайлИзВнешнегоЗипФайла
			s = p[0];
			ss = p[1];
			sss = p[2];
			zip = NULL;
			Helper = NULL;
			try
			{
				zip = new TAbZipArchive(s, fmOpenRead);
				THelper* Helper = new THelper;
				zip->ExtractHelper = Helper->UnzipProc;
				zip->Load();
				CreateDirectory(ss.c_str(), NULL);
				zip->BaseDirectory = ss;
				zip->ExtractOptions >> eoCreateDirs >> eoRestorePath;
				zip->ExtractFiles(sss);
				delete Helper;
				delete zip;
			}
			catch(...)
			{
				delete Helper;
				delete zip;
				ret->Clear();
				return S_OK;
			}
			*ret = true;
			break;
		default:
			ret->Clear();
	}

	return S_OK;
}

STDMETHODIMP TCv8cf2Impl::CallAsProc(long lMethodNum, LPSAFEARRAY* paParams)
{
	//MessageBox(NULL, "CallAsProc", "Ура", MB_OK);
	VARIANT pvarRetValue;
	return CallAsFunc(lMethodNum, &pvarRetValue, paParams);
}

// Найти метод
STDMETHODIMP TCv8cf2Impl::FindMethod(BSTR bstrMethodName, long* plMethodNum)
{
	//MessageBox(NULL, "FindMethod", "Ура", MB_OK);
	AnsiString s = AnsiString(bstrMethodName).UpperCase();
	for(int i = 0; i < nMet; i++)
	{
		if(methods[i].name.UpperCase() == s || methods[i].synonym.UpperCase() == s)
		{
			*plMethodNum = i;
			return S_OK;
		}
	}
	*plMethodNum = -1;
	return S_OK;
}

// Найти свойство
STDMETHODIMP TCv8cf2Impl::FindProp(BSTR bstrPropName, long* plPropNum)
{
	//MessageBox(NULL, "FindProp", "Ура", MB_OK);
	AnsiString s = AnsiString(bstrPropName).UpperCase();
	for(int i = 0; i < nMet; i++)
	{
		if(properties[i].name.UpperCase() == s || properties[i].synonym.UpperCase() == s)
		{
			*plPropNum = i;
			return S_OK;
		}
	}
	*plPropNum = -1;
	return S_FALSE;
}

// Получить имя метода
STDMETHODIMP TCv8cf2Impl::GetMethodName(long lMethodNum, long lMethodAlias, BSTR* pbstrMethodName)
{
	//MessageBox(NULL, "GetMethodName", "Ура", MB_OK);
	if(lMethodNum >= nMet) return S_FALSE;
	if(lMethodAlias) methods[lMethodNum].name.WideChar(wbuf, wbuf_len);
	else methods[lMethodNum].synonym.WideChar(wbuf, wbuf_len);
	*pbstrMethodName = SysAllocString(wbuf);
	return S_OK;
}

// Получить количество методов
STDMETHODIMP TCv8cf2Impl::GetNMethods(long* plMethods)
{
	//MessageBox(NULL, "GetNMethods", "Ура", MB_OK);
	*plMethods = nMet;
	return S_OK;
}

// получить количество параметров метода
STDMETHODIMP TCv8cf2Impl::GetNParams(long lMethodNum, long* plParams)
{
	//MessageBox(NULL, "GetNParams", "Ура", MB_OK);
	if(lMethodNum >= nMet)
	{
		*plParams = 0;
		return S_FALSE;
	}
	*plParams = methods[lMethodNum].nParam;
	return S_OK;
}

// Получить количество свойств
STDMETHODIMP TCv8cf2Impl::GetNProps(long* plProps)
{
	//MessageBox(NULL, "GetNProps", "Ура", MB_OK);
	*plProps = nProp;
	return S_OK;
}

//
STDMETHODIMP TCv8cf2Impl::GetParamDefValue(long lMethodNum, long lParamNum, VARIANT* pvarParamDefValue)
{
	//MessageBox(NULL, "GetParamDefValue", "Ура", MB_OK);
	(*(Variant*)pvarParamDefValue).Clear();
	if(lMethodNum >= nMet) return S_FALSE;
	if(lParamNum >= methods[lMethodNum].nParam) return S_FALSE;
	*(Variant*)pvarParamDefValue = methods[lMethodNum].params[lParamNum].DefValue;
	return S_OK;
}

// Получить имя свойства
STDMETHODIMP TCv8cf2Impl::GetPropName(long lPropNum, long lPropAlias, BSTR* pbstrPropName)
{
	//MessageBox(NULL, "GetPropName", "Ура", MB_OK);
	if(lPropNum >= nProp) return S_FALSE;
	if(lPropAlias) properties[lPropNum].name.WideChar(wbuf, wbuf_len);
	else properties[lPropNum].synonym.WideChar(wbuf, wbuf_len);
	*pbstrPropName = SysAllocString(wbuf);
	return S_OK;
}

// Получить значение свойства
STDMETHODIMP TCv8cf2Impl::GetPropVal(long lPropNum, VARIANT* pvarPropVal)
{
	//MessageBox(NULL, "GetPropVal", "Ура", MB_OK);

	if(lPropNum >= nProp) return S_FALSE;
	if(!properties[lPropNum].readable) return S_FALSE;

	switch(lPropNum){
		case 0: // Открыт
			*(Variant*)pvarPropVal = !(v8cat == NULL);
			break;
		case 1:
			if(!v8cat) (*(Variant*)pvarPropVal).Clear();
			else *(Variant*)pvarPropVal = name.c_str();
			break;
	}
	return S_OK;
}

STDMETHODIMP TCv8cf2Impl::HasRetVal(long lMethodNum, long* pboolRetValue)
{
	//MessageBox(NULL, "HasRetVal", "Ура", MB_OK);
	if(lMethodNum >= nMet) return S_FALSE;
	*pboolRetValue = methods[lMethodNum].hasRetVal;
	return S_OK;
}

// проверка читабельности свойства
STDMETHODIMP TCv8cf2Impl::IsPropReadable(long lPropNum, long* pboolPropRead)
{
	//MessageBox(NULL, "IsPropReadable", "Ура", MB_OK);
	if(lPropNum >= nMet) return S_FALSE;
	*pboolPropRead = properties[lPropNum].readable;
	return S_OK;
}

// проверка записываемости свойства
STDMETHODIMP TCv8cf2Impl::IsPropWritable(long lPropNum, long* pboolPropWrite)
{
	//MessageBox(NULL, "IsPropWritable", "Ура", MB_OK);
	if(lPropNum >= nMet) return S_FALSE;
	*pboolPropWrite = properties[lPropNum].writable;
	return S_OK;
}

STDMETHODIMP TCv8cf2Impl::SetPropVal(long lPropNum, VARIANT* varPropVal)
{
	//MessageBox(NULL, "SetPropVal", "Ура", MB_OK);

	return S_FALSE;
}


