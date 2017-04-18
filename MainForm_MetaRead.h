//---------------------------------------------------------------------------

#ifndef MainForm_MetaReadH
#define MainForm_MetaReadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvEdBtn.hpp"
#include "AdvEdit.hpp"
#include "AdvFileNameEdit.hpp"
#include <Vcl.ComCtrls.hpp>
#include "TreeList.hpp"
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
#include "MessageRegistration.h"
#include "MetaData.h"
#include "AdvDirectoryEdit.hpp"
#include <Vcl.Grids.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include "acAlphaImageList.hpp"
#include "AdvMemo.hpp"
#include "AdvCombo.hpp"
#include <Soap.WebServExp.hpp>
#include <Soap.WSDLBind.hpp>
#include <Soap.WSDLPub.hpp>
#include <System.TypInfo.hpp>
#include <Xml.XMLSchema.hpp>
#include "AdvmWS.hpp"
#include "Advmxml.hpp"
#include "AdvPanel.hpp"
#include "HTMLText.hpp"
#include "HTMLabel.hpp"
#include "HTMLForm.hpp"
#include <SHDocVw.hpp>
#include <Vcl.OleCtrls.hpp>
#include "AdvReflectionImage.hpp"
#include <IdBaseComponent.hpp>
#include <IdCompressorZLib.hpp>
#include <IdZLibCompressorBase.hpp>
#include "WebImage.hpp"
//---------------------------------------------------------------------------
class Messager : public MessageRegistrator
{
private:
	String logfile;
	TFormatSettings FormatSettings;
	TMemo* MemoLog;
	TCriticalSection *Lock;
	void __fastcall initFormatSettings();
	String const* _s;
public:
	__fastcall Messager();
	__fastcall Messager(const String& _logfile, TMemo* _MemoLog);
	__fastcall ~Messager();
	void __fastcall setlogfile(const String& _logfile);
	virtual void __fastcall AddMessage(const String& message, const MessageState mstate, TStringList* param = NULL);
	virtual void __fastcall Status(const String& message);
	void __fastcall mainthreadAddMessage();
	void __fastcall mainthreadStatus();
};
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonReadTypes;
	TPageControl *PageControlMain;
	TTabSheet *TabSheetTypes;
	TTabSheet *TabSheetConfig;
	TTreeView *TreeViewTypes;
	TAdvDirectoryEdit *AdvDirectoryEditStorageDirectory;
	TButton *ButtonLoadStorage;
	TTabSheet *TabSheetLog;
	TMemo *MemoLog;
	TPageControl *PageControlConf;
	TTabSheet *TabSheetGeneratedTypes;
	TTabSheet *TabSheetMeta;
	TAdvStringGrid *AdvStringGridGenTypes;
	TAdvStringGrid *AdvStringGridMeta;
	TTabSheet *TabSheetConf;
	TVirtualStringTree *VirtualStringTreeValue1C;
	TPanel *PanelСurConfPath;
	TAdvFileNameEdit *AdvFileNameEditConfigFile;
	TsAlphaImageList *sAlphaImageListConfig;
	TCheckBox *CheckBoxUseImages;
	TCheckBox *CheckBoxEnglish;
	TSplitter *SplitterConf;
	TPanel *PanelStatusBar;
	TAdvMemo *MemoValue;
	TPanel *Panel2;
	TPanel *PanelViewIn1C;
	TButton *ButtonViewIn1C;
	TAdvComboBox *AdvComboBoxVariant;
	TAdvFileNameEdit *AdvFileNameEditBaseFile;
	TCheckBox *CheckBoxLogToFile;
	TAdvFileNameEdit *AdvFileNameEditLogFile;
	TTabSheet *TabSheetParam;
	TCheckBox *CheckBoxUseExternal;
	TPageControl *PageControlFileFormats;
	TTabSheet *TabSheetText;
	TTabSheet *TabSheetPicture;
	TScrollBox *ScrollBox1;
	TAdvXMLMemoStyler *AdvXMLMemoStyler1;
	TAdvHTMLMemoStyler *AdvHTMLMemoStyler1;
	TTabSheet *TabSheetHTML;
	TWebBrowser *WebBrowserHTML;
	TAdvReflectionImage *AdvReflectionImagePicture;
	TTabSheet *TabSheetExport;
	TAdvDirectoryEdit *AdvDirectoryEditExport;
	TButton *ButtonExport;
	TAdvEdit *AdvEditExportThreadCount;
	void __fastcall ButtonReadTypesClick(TObject *Sender);
	void __fastcall ButtonLoadStorageClick(TObject *Sender);
	void __fastcall VirtualStringTreeValue1CInitChildren(TBaseVirtualTree *Sender, PVirtualNode Node, DWORD &ChildCount);
	void __fastcall VirtualStringTreeValue1CGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
		  UnicodeString &CellText);
	void __fastcall VirtualStringTreeValue1CInitNode(TBaseVirtualTree *Sender, PVirtualNode ParentNode, PVirtualNode Node, TVirtualNodeInitStates &InitialStates);
	void __fastcall VirtualStringTreeValue1CFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);
	void __fastcall VirtualStringTreeValue1CGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
		  bool &Ghosted, int &ImageIndex);
	void __fastcall CheckBoxUseImagesClick(TObject *Sender);
	void __fastcall ButtonViewIn1CClick(TObject *Sender);
	void __fastcall AdvComboBoxVariantChange(TObject *Sender);
	void __fastcall CheckBoxLogToFileClick(TObject *Sender);
	void __fastcall ButtonExportClick(TObject *Sender);


private:	// User declarations
	Messager* mess;
	MetaContainer* Container;
	void __fastcall showValue1C();
	void __fastcall showGenTypes();
	void __fastcall showMeta();
	String __fastcall getNodeName(PVirtualNode Node);
	String __fastcall getNodeValue(PVirtualNode Node);
	String __fastcall getNodeType(PVirtualNode Node);

	String s_1cv8fv; // Путь к файлу 1cv8fv.exe (например, "C:\Program Files (x86)\1cv82fv\bin\1cv8fv.exe"), если существует
	bool aboutblank;
public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);
	__fastcall ~TFormMain();
};

struct VirtualTreeData
{
	MetaProperty* p;
	Value1C* v;
	int q; // Дополнтельный признак узла. 0 - значение не установлено, 1 - обычный узел, 2,3 - особый способ вычисления имени (подчиненный узел узла СтрокаДереваЗначений (3) или СтрокаТаблицыЗначений (2) - имя вычисляется как имя колонки)
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif

