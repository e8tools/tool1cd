//---------------------------------------------------------------------------

#ifndef ICUH
#define ICUH

#ifndef PublicRelease

#ifndef delic
#include "ICU\34\include\icu34.h"
#include "ICU\40\include\icu40.h"
#endif
#include "ICU\46\include\icu46.h"

#include "db_ver.h"

//---------------------------------------------------------------------------
enum ICU_Result
{
	r_OK,
	r_notInit,
	r_badLocale,
	r_LocaleNotSet,
	r_keyTooSmall,
	r_badVersion
};

class ICU
{
private:
#ifndef delic
	UCollator* coll34;
	UCollator* coll40;
#endif
	UCollator* coll46;
	db_ver ver;

	void __fastcall close_collator();

public:
	__fastcall ICU(db_ver _ver);
	__fastcall ~ICU();
	ICU_Result __fastcall setLocale(const char* loc);
	ICU_Result __fastcall getSortKey(const wchar_t* str, uint8_t* key, int keysize, int& realkeysize, bool CaseSensitive);
	ICU_Result __fastcall getSortKey(const wchar_t* str, int strlen, uint8_t* key, int keysize, int& realkeysize, bool CaseSensitive);
};


//---------------------------------------------------------------------------
#endif //#ifdef PublicRelease

#endif //#ifndef ICUH

