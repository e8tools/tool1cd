#ifndef cunicodeH
#define cunicodeH

#include <windows.h>

#define wdirtypemax 1024
#define longnameprefixmax 6

#ifndef countof
#define countof(str) (sizeof(str)/sizeof(str[0]))
#endif // countof

wchar_t* wcslcpy(wchar_t *str1,const wchar_t *str2,int imaxlen);
wchar_t* wcslcat(wchar_t *str1,const wchar_t *str2,int imaxlen);
char* walcopy(char* outname,wchar_t* inname,int maxlen);
wchar_t* awlcopy(wchar_t* outname,char* inname,int maxlen);

#define wafilenamecopy(outname,inname) walcopy(outname,inname,countof(outname)-1)
#define awfilenamecopy(outname,inname) awlcopy(outname,inname,countof(outname)-1)

#endif

