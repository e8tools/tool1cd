#include "cunicode.h"
#include <string.h>
#include <wchar.h>

char* walcopy(char* outname,wchar_t* inname,int maxlen)
{
	if (inname) {
		WideCharToMultiByte(CP_ACP,0,inname,-1,outname,maxlen,NULL,NULL);
		outname[maxlen]=0;
		return outname;
	} else
		return NULL;
}

wchar_t* awlcopy(wchar_t* outname,char* inname,int maxlen)
{
	if (inname) {
		MultiByteToWideChar(CP_ACP,0,inname,-1,outname,maxlen);
		outname[maxlen]=0;
		return outname;
	} else
		return NULL;
}

wchar_t* wcslcpy(wchar_t *str1,const wchar_t *str2,int imaxlen)
{
	if ((int)wcslen(str2)>=imaxlen-1) {
#if defined (_MSC_VER)
		wcsncpy_s(str1, imaxlen - 1, str2, imaxlen - 1);
#else
		wcsncpy(str1,str2,imaxlen-1);
#endif
		str1[imaxlen-1]=0;
	} else
#if defined (_MSC_VER)
		wcscpy_s(str1, imaxlen - 1, str2);
#else
		wcscpy(str1,str2);
#endif

	return str1;
}

wchar_t* wcslcat(wchar_t *str1,const wchar_t *str2,int imaxlen)
{
	int l1=(int)wcslen(str1);
	if ((int)wcslen(str2)+l1>=imaxlen-1) {
#if defined (_MSC_VER)
		wcsncpy_s(str1 + l1, imaxlen - 1 - l1,str2, imaxlen - 1 - l1);
#else
		wcsncpy(str1 + l1, str2, imaxlen - 1 - l1);
#endif

		
		str1[imaxlen-1]=0;
	} else
#if defined (_MSC_VER)
		wcscat_s(str1, imaxlen - 1, str2);
#else
		wcscat(str1, str2);
#endif
	
	return str1;
}


