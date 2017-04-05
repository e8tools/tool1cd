//---------------------------------------------------------------------------

#pragma hdrstop

#include "ICU.h"

#ifndef PublicRelease

//---------------------------------------------------------------------------

__fastcall ICU::ICU(db_ver _ver)
{
#ifndef delic
	coll34 = NULL;
	coll40 = NULL;
#endif
	coll46 = NULL;

	ver = _ver;

}

//---------------------------------------------------------------------------
__fastcall ICU::~ICU()
{
#ifndef delic
	if(coll34) ucol_close_3_4(coll34);
	if(coll40) ucol_close_4_0(coll40);
#endif
	if(coll46) ucol_close_46(coll46);

}
//---------------------------------------------------------------------------
void __fastcall ICU::close_collator()
{
#ifndef delic
	if(coll34)
	{
		ucol_close_3_4(coll34);
		coll34 = NULL;
	}
	if(coll40)
	{
		ucol_close_4_0(coll40);
		coll40 = NULL;
	}
#endif
	if(coll46)
	{
		ucol_close_46(coll46);
		coll46 = NULL;
	}

}

//---------------------------------------------------------------------------
ICU_Result __fastcall ICU::setLocale(const char* loc)
{
#ifndef delic
	UErrorCode_3_4_ status34 = U_ZERO_ERROR_3_4_;
	UErrorCode_4_0_ status40 = U_ZERO_ERROR_4_0_;
#endif
	UErrorCode_4_6_ status46 = U_ZERO_ERROR_4_6_;

	close_collator();

	if(loc == NULL) return r_badLocale;
	//if(strlen(loc) == 0) return r_badLocale;
	if(loc[0] == '\0') return r_badLocale;

	switch(ver)
	{
#ifndef delic
		case ver8_1_0_0:
			coll34 = ucol_open_3_4(loc, &status34);
			if(!U_SUCCESS(status34)) {
				coll34 = NULL;
				return r_LocaleNotSet;
			}
			return r_OK;

		case ver8_2_0_0:
			coll40 = ucol_open_4_0(loc, &status40);
			if(!U_SUCCESS(status40)) {
				coll40 = NULL;
				return r_LocaleNotSet;
			}
			return r_OK;

#endif
		case ver8_2_14_0:
		case ver8_3_8_0:
			coll46 = ucol_open_46(loc, &status46);
			if(!U_SUCCESS(status46)) {
				coll46 = NULL;
				return r_LocaleNotSet;
			}
			return r_OK;

		default:
			return r_badVersion;

	}

}


//---------------------------------------------------------------------------
ICU_Result __fastcall ICU::getSortKey(const wchar_t* str, uint8_t* key, int keysize, int& realkeysize, bool CaseSensitive)
{
	return getSortKey(str, -1, key, keysize, realkeysize, CaseSensitive);
}

//---------------------------------------------------------------------------
ICU_Result __fastcall ICU::getSortKey(const wchar_t* str, int strlen, uint8_t* key, int keysize, int& realkeysize, bool CaseSensitive)
{

	switch(ver)
	{
#ifndef delic
		case ver8_1_0_0:
			if(!coll34) return r_notInit;
			ucol_setStrength_3_4(coll34, CaseSensitive ? UCOL_TERTIARY_3_4_ : UCOL_SECONDARY_3_4_);
			realkeysize = ucol_getSortKey_3_4(coll34, str, strlen, key, keysize);
			break;

		case ver8_2_0_0:
			if(!coll40) return r_notInit;
			ucol_setStrength_4_0(coll40, CaseSensitive ? UCOL_TERTIARY_4_0_ : UCOL_SECONDARY_4_0_);
			realkeysize = ucol_getSortKey_4_0(coll40, str, strlen, key, keysize);
			break;

#endif
		case ver8_2_14_0:
		case ver8_3_8_0:
			if(!coll46) return r_notInit;
			ucol_setStrength_46(coll46, CaseSensitive ? UCOL_TERTIARY_4_6_ : UCOL_SECONDARY_4_6_);
			realkeysize = ucol_getSortKey_46(coll46, str, strlen, key, keysize);
			break;

		default:
			return r_badVersion;

	}

	if(realkeysize > keysize)
	{
		return r_keyTooSmall;
	}

	return r_OK;

}

//---------------------------------------------------------------------------

#pragma package(smart_init)

#endif //#ifdef PublicRelease

