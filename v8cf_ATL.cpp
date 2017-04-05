//$$---- ATL Unit CPP ---- (stATLUnitSource)
//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop

#define USING_ATL

#include "v8cf_ATL.h"

#pragma option -w-aus		// Var. assigned value never used

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atl\atlimpl.cpp>

#if defined(USING_ATLVCL)
#include <atl\atlvcl.cpp>
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
