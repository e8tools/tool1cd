//$$---- ATL unit HDR ---- (stATLUnitHdr)
//---------------------------------------------------------------------------


#ifndef v8cf_ATLH
#define v8cf_ATLH

// Check if we're using ATLVCL
//
#if defined(USING_ATLVCL)

// To get declaration of _Module
//
#include <atl\atlvcl.h>

#elif defined(USING_ATL)

// Light-weight ATL support
//

#include <utilcls.h>	// _ASSERTE
#include <atlbase.h>

// NOTE: You may derive a class from TComModule and use it as your server's
//       module object. (i.e. Assign an instance of that class to _Module).
//       However, you must *NOT* change the name _Module.
//
extern CComModule &_Module;

#include <atlcom.h>
#include <shellapi.h>

#endif


#endif	// Project2_ATLH
