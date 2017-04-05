// CV8CFIMPL.H : Declaration of the TCv8cf2Impl

#ifndef Cv8cfImplH
#define Cv8cfImplH

#define ATL_APARTMENT_THREADED

#include "Addin_TLB.H"

#include "v8cf_extcomp.H"
#include "APIcfBase.h"


/////////////////////////////////////////////////////////////////////////////
// TCv8cf2Impl     Implements ICv8cf, default interface of Cv8cf
// ThreadingModel : Apartment
// Dual Interface : FALSE
// Event Support  : FALSE
// Default ProgID : Addin.Cv8cf2
// Description    :
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE TCv8cf2Impl :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<TCv8cf2Impl, &CLSID_Cv8cf2>,
  public IInitDone,
  public ILanguageExtender
{
public:

	static method methods[]; // массив описаний методов
	static int nMet; // количество методов
	static property properties[]; // массив описаний свойств
	static int nProp; // количество свойств

	v8catalog* v8cat; // каталог файла (если файл не открыт, то NULL)
	AnsiString name; // полное имя открытого файла

	TCv8cf2Impl()
	{
		v8cat = NULL;
	}

	~TCv8cf2Impl()
	{
		if(v8cat) delete v8cat;
	}

	// Data used when registering Object
	//
	DECLARE_THREADING_MODEL(otApartment);
	DECLARE_PROGID("Addin.Cv8cf2");
	DECLARE_DESCRIPTION("v8cf");

	// Function invoked to (un)register object
	//
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
	{
		TTypedComServerRegistrarT<TCv8cf2Impl>
		regObj(GetObjectCLSID(), GetProgID(), GetDescription());
		return regObj.UpdateRegistry(bRegister);
	}


BEGIN_COM_MAP(TCv8cf2Impl)
	COM_INTERFACE_ENTRY(IInitDone)
	COM_INTERFACE_ENTRY(ILanguageExtender)
END_COM_MAP()

// ICv8cf
public:

  STDMETHOD(Done());
  STDMETHOD(GetInfo(LPSAFEARRAY* pInfo));
  STDMETHOD(Init(LPDISPATCH pConnection));
  STDMETHOD(RegisterExtensionAs(BSTR* pExtensionName));
  STDMETHOD(CallAsFunc(long lMethodNum, VARIANT* pvarRetValue, LPSAFEARRAY* paParams));
  STDMETHOD(CallAsProc(long lMethodNum, LPSAFEARRAY* paParams));
  STDMETHOD(FindMethod(BSTR bstrMethodName, long* plMethodNum));
  STDMETHOD(FindProp(BSTR bstrPropName, long* plPropNum));
  STDMETHOD(GetMethodName(long lMethodNum, long lMethodAlias, BSTR* pbstrMethodName));
  STDMETHOD(GetNMethods(long* plMethods));
  STDMETHOD(GetNParams(long lMethodNum, long* plParams));
  STDMETHOD(GetNProps(long* plProps));
  STDMETHOD(GetParamDefValue(long lMethodNum, long lParamNum, VARIANT* pvarParamDefValue));
  STDMETHOD(GetPropName(long lPropNum, long lPropAlias, BSTR* pbstrPropName));
  STDMETHOD(GetPropVal(long lPropNum, VARIANT* pvarPropVal));
  STDMETHOD(HasRetVal(long lMethodNum, long* pboolRetValue));
  STDMETHOD(IsPropReadable(long lPropNum, long* pboolPropRead));
  STDMETHOD(IsPropWritable(long lPropNum, long* pboolPropWrite));
  STDMETHOD(SetPropVal(long lPropNum, VARIANT* varPropVal));
};

#endif //Cv8cfImplH

