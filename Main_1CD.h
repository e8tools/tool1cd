//---------------------------------------------------------------------------

#ifndef Main_1CDH
#define Main_1CDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ValEdit.hpp>
#include <CategoryButtons.hpp>
#include <ButtonGroup.hpp>
#include "ATViewer.hpp"
#include <ActnMan.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include "AdvGrid.hpp"
#include "BaseGrid.hpp"
#include "MessageRegistration.h"
#include "Class_1CD.h"
#include "TableReadThread.h"
#include <xmldom.hpp>
#include <Xmlxform.hpp>
#include "AdvMemo.hpp"
#include "AdvmWS.hpp"
#include "AdvGlowButton.hpp"
#include "AdvSmoothButton.hpp"
#include "GDIPPictureContainer.hpp"
#include "AdvGlassButton.hpp"
#include "AdvDirectoryEdit.hpp"
#include "AdvEdBtn.hpp"
#include "AdvEdit.hpp"
#include "AdvToolBtn.hpp"
#include "frmctrllink.hpp"
#include <ActnCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include "MaskEdEx.hpp"
#include <Mask.hpp>
#include "ParseCommandLine.h"
#include <iostream.h>
#include "AdvFileNameEdit.hpp"
#include "AdvObj.hpp"
#include <System.Actions.hpp>
#include "awaMaskEdit.h"
#include "HTMLabel.hpp"
#include "PictureContainer.hpp"

//---------------------------------------------------------------------------
class Messager;
class TableReadThread;

class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *StatusBar1;
	TListView *ListViewMessage;
	TImageList *ImageListMessages;
	TButtonedEdit *ButtonedEditFile;
	TActionList *ActionListMain;
	TImageList *ImageListMain;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TAction *ActionFileOpen;
	TMenuItem *N_FileOpen;
	TOpenDialog *OpenDialog;
	TMenuItem *N_Exit;
	TAction *ActionExit;
	TPageControl *PageControl1;
	TTabSheet *TabSheetTest;
	TTabSheet *TabSheetView;
	TPanel *PanelMessages;
	TValueListEditor *ValueListEditorMessage;
	TPanel *Panel3;
	TLabel *Label1;
	TMemo *MemoDescriptionTable;
	TPanel *PanelDescriptionTable;
	TMenuItem *N2;
	TMenuItem *N3;
	TAction *ActionDebugMessages;
	TPanel *PanelBlobView;
	TPanel *PanelBlobViewProperties;
	TListView *ListViewTables;
	TPageControl *PageControlTableView;
	TTabSheet *TabSheetDescription;
	TTabSheet *TabSheetPhysicalView;
	TComboBoxEx *ComboBoxExBlobViewMode;
	TGroupBox *GroupBoxBlobViewMode;
	TAction *ActionBlobViewModeDetect;
	TAction *ActionBlobViewANSI;
	TAction *ActionBlobViewUTF8;
	TAction *ActionBlobViewUTF16;
	TMenuItem *N4;
	TMenuItem *N9;
	TMenuItem *N10;
	TMenuItem *N11;
	TMenuItem *N12;
	TAction *ActionBlobView;
	TAction *ActionBlobViewMedia;
	TAction *ActionBlobViewWEB;
	TAction *ActionBlobViewHEX;
	TMenuItem *N13;
	TMenuItem *N14;
	TMenuItem *N15;
	TMenuItem *N5;
	TAction *ActionBlobViewTextWrap;
	TAction *ActionBlobViewMode;
	TMenuItem *N6;
	TMenuItem *N7;
	TPanel *PanelContainerList;
	TLabel *Label4;
	TAction *ActionBlobParseModeDetect;
	TAction *ActionBlobParseUnzip;
	TAction *ActionBlobParseContainer;
	TMenuItem *N8;
	TMenuItem *N16;
	TMenuItem *N17;
	TMenuItem *N18;
	TMenuItem *ActionBlobParseContainer1;
	TGroupBox *GroupBoxBlobParseMode;
	TAction *ActionBlobParseUnzip2;
	TTreeView *TreeViewContainer;
	TMenuItem *N2Unzip21;
	TAction *ActionBlobParseValueStorage;
	TMenuItem *N19;
	TAction *ActionBlobParseValue1C;
	TMenuItem *N110;
	TMenuItem *N20;
	TPanel *Panel2;
	TPanel *Panel5;
	TPanel *Panel6;
	TPanel *Panel7;
	TValueListEditor *ValueListEditorDescriprionTable;
	TLabel *Label2;
	TLabel *Label5;
	TLabel *Label6;
	TTreeView *TreeViewDescriptionIndexes;
	TAction *ActionSaveConfig;
	TAction *ActionSaveConfigSave;
	TSaveDialog *SaveDialogCF;
	TMenuItem *N21;
	TMenuItem *N22;
	TMenuItem *N23;
	TTimer *TimerGarbageMemory;
	TAdvStringGrid *StringGridPhysicalView;
	TAdvStringGrid *StringGridDescriptionFields;
	TButton *ButtonSaveConfigSave;
	TButton *ButtonSaveConfig;
	TAction *ActionFindSupplierConfigs;
	TButton *sButton3;
	TAdvStringGrid *StringGridSupplierConfigs;
	TPageControl *PageControlUtils;
	TTabSheet *TabSheetConfigSave;
	TPanel *PanelSupplierConfigs;
	TLabel *sLabel1;
	TPanel *sPanel3;
	TAction *ActionSaveSupplierConfig;
	TButton *sButton4;
	TMenuItem *N24;
	TAction *ActionSaveDataFile;
	TAction *ActionSaveBlobFile;
	TAction *ActionSaveIndexFile;
	TMenuItem *Records1;
	TMenuItem *Blob1;
	TMenuItem *Index1;
	TSaveDialog *SaveDialogFile;
	TAction *ActionDumpIndex;
	TButton *sButton6;
	TAction *ActionExportTableToXML;
	TSaveDialog *SaveDialogXML;
	TAction *ActionAbout;
	TMenuItem *N25;
	TMenuItem *N26;
	TMenuItem *XML1;
	TAction *ActionViewFileOffset;
	TMenuItem *ActionViewFileOffset1;
	TATViewer *ATViewerBlob;
	TAction *ActionShowGUIDs;
	TAction *ActionOpenBaseNotMonopoly;
	TMenuItem *N27;
	TMenuItem *Bynary16GUID1;
	TAction *ActionTestIndexes;
	TAction *ActionEditTable;
	TAction *ActionSaveTableChanges;
	TAction *ActionDiscardTableChanges;
	TTabSheet *TabSheetUtilities;
	TAdvDirectoryEdit *DirectoryEditExportImportTable;
	TAction *ActionExportTable;
	TAction *ActionImportTable;
	TAdvToolButton *AdvToolButton1;
	TAdvToolButton *AdvToolButton2;
	TAdvToolButton *AdvToolButton3;
	TAction *ActionExportDataTables;
	TAction *ActionImportDataTables;
	TAdvToolButton *AdvToolButton4;
	TAdvToolButton *AdvToolButton5;
	TFormControlEditLink *FormControlEditLinkCommon;
	TPanel *PanelPhysView;
	TLabel *LabelPhysPreview;
	TToolBar *ToolBarPhysView;
	TToolButton *ToolButtonEditTable;
	TToolButton *ToolButtonSaveTableChanges;
	TToolButton *ToolButtonDiscardTableChanges;
	TToolButton *ToolButtonInsDelSeparator;
	TToolButton *ToolButtonEditSeparator;
	TToolButton *ToolButtonExportTableToXML;
	TComboBox *ComboBoxCurrentIndex;
	TAction *ActionEditTableInsert;
	TAction *ActionEditTableDelete;
	TToolButton *ToolButtonExportXMLSeparator;
	TToolButton *ToolButtonEditTableInsert;
	TToolButton *ToolButtonEditTableDelete;
	TTabSheet *TabSheetStatistic;
	TAdvStringGrid *AdvStringGridStatistic;
	TPanel *PanelEditDropDown;
	TAction *ActionStringEditDropDown;
	TButtonedEdit *ButtonedEditCommonEdit;
	TImageList *ImageListEdits;
	TAdvToolButton *AdvToolButtonStringNULL;
	TAction *ActionNULL;
	TAction *ActionRestore;
	TAdvToolButton *AdvToolButtonStringRestore;
	TAdvToolButton *AdvToolButtonStringClear;
	TAction *ActionClear;
	TAction *ActionFindLostObjects;
	TAdvToolButton *AdvToolButton6;
	TAction *ActionTestStreamFormat;
	TAdvToolButton *AdvToolButton7;
	TAction *ActionCreateEmptyObject;
	TAdvToolButton *AdvToolButton8;
	TAction *ActionCreateObjectFromFile;
	TAdvToolButton *AdvToolButton9;
	TOpenDialog *OpenDialogAnyFile;
	TAction *ActionCreateAndImportTable;
	TAdvToolButton *AdvToolButton10;
	TAdvDirectoryEdit *DirectoryEditCreateImportTable;
	TAction *ActionDeleteTable;
	TAction *ActionEnableEdit;
	TAdvToolButton *AdvToolButton11;
	TAction *ActionTestListTables;
	TAdvToolButton *AdvToolButton12;
	TCheckBox *CheckBoxBlobParseValueStorage;
	TCheckBox *CheckBoxBlobParseModeDetect;
	TCheckBox *CheckBoxBlobParseValue1C;
	TCheckBox *CheckBoxBlobParseUnzip;
	TCheckBox *CheckBoxBlobParseUnzip2;
	TCheckBox *CheckBoxBlobParseUsersUsr;
	TCheckBox *CheckBoxBlobViewModeAuto;
	TCheckBox *CheckBoxBlobViewTextWrap;
	TSplitter *SplitterBlobView;
	TCheckBox *CheckBoxSortListTables;
	TSplitter *Splitter7;
	TSplitter *SplitterContainerList;
	TSplitter *SplitterBlobViewProperties;
	TSplitter *Splitter3;
	TSplitter *Splitter2;
	TSplitter *Splitter1;
	TSplitter *Splitter5;
	TSplitter *Splitter6;
	TAction *ActionCreateTables;
	TAdvToolButton *AdvToolButton13;
	TAction *ActionReplaceTREF;
	TAdvFileNameEdit *AdvFileNameEditReplaceTREF;
	TAdvToolButton *AdvToolButton14;
	TAction *ActionBlobParseUsersUsr;
	TCheckBox *CheckBoxBlobParseContainer;
	TAction *ActionAssociateFile;
	TMenuItem *N1CDTool1CD1;
	TMenuItem *N28;
	TAction *ActionXMLSaveBLOBToFile;
	TMenuItem *XMLBLOB1;
	TAction *ActionXMLUnpackBLOB;
	TMenuItem *XMLBLOB2;
	TAdvToolButton *AdvToolButton15;
	TAction *ActionFindAndCreateLostTables;
	TAdvToolButton *AdvToolButton16;
	TAction *ActionDeleteWrongTables;
	TAdvToolButton *AdvToolButton17;
	TAction *ActionFindTableDoubles;
	TAdvToolButton *AdvToolButton18;
	TAction *ActionFindAndSaveLostObjects;
	TAdvToolButton *AdvToolButton19;
	TMenuItem *N29;
	TAction *ActionCommandLineKeys;
	TMenuItem *ActionCommandLineKeys1;
	TAction *ActionTestStreamFormatExternal;
	TAdvToolButton *AdvToolButton20;
	TOpenDialog *OpenDialogExt;
	TPopupMenu *EmptyPopupMenu;
	TFormControlEditLink *FormControlEditLinkMasked;
	TawaMaskEdit *MaskEditCommonEdit;
	TPanel *PanelBlobTools;
	TToolBar *ToolBarBlobViewTools;
	TToolButton *ToolButtonWriteBlobToFile;
	TToolButton *ToolButtonReadBlobFromFile;
	TToolButton *ToolButtonBlobClear;
	TAction *ActionWriteBlobToFile;
	TAction *ActionReadBlobFromFile;
	TAction *ActionBlobClear;
	TAction *ActionBlobRestore;
	TToolButton *ToolButtonBlobRestore;
	TPanel *Panel1;
	TLabel *LabelCurrentTable;
	TTabSheet *TabSheetDepotConfigSave;
	TButton *ButtonSaveLastDepotConfig;
	TAction *ActionSaveLastDepotConfig;
	TMenuItem *N30;
	TAction *ActionSaveDepotConfig;
	TToolButton *ToolButtonSaveDepotConfig;
	THTMLabel *HTMLabel1;
	TAdvToolButton *AdvToolButton21;
	TAction *ActionRestoreDATAAllocationTable;
	TAction *ActionSaveConfigExtension;
	TAction *ActionSaveConfigExtensionDb;
	TSaveDialog *SaveDialogCFE;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TPictureContainer *PictureContainer1;
	THTMLabel *HTMLabel2;
	TMenuItem *N31;
	TAction *ActionRecoveryMode;
	void __fastcall ActionFileOpenExecute(TObject *Sender);
	void __fastcall ActionExitExecute(TObject *Sender);
	void __fastcall ListViewMessageSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall CheckBoxSortListTables_Click(TObject *Sender);
	void __fastcall ActionDebugMessagesExecute(TObject *Sender);
	void __fastcall StringGridPhysicalViewSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect);
	void __fastcall ListViewTablesCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
		  int Data, int &Compare);
	void __fastcall PageControlTableViewChange(TObject *Sender);
	void __fastcall ListViewTablesSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall ActionBlobViewANSIExecute(TObject *Sender);
	void __fastcall ActionBlobViewUTF8Execute(TObject *Sender);
	void __fastcall ActionBlobViewUTF16Execute(TObject *Sender);
	void __fastcall ActionBlobViewModeDetectExecute(TObject *Sender);
	void __fastcall ActionBlobViewExecute(TObject *Sender);
	void __fastcall ActionBlobViewMediaExecute(TObject *Sender);
	void __fastcall ActionBlobViewWEBExecute(TObject *Sender);
	void __fastcall ActionBlobViewHEXExecute(TObject *Sender);
	void __fastcall ComboBoxExBlobViewModeChange(TObject *Sender);
	void __fastcall ActionBlobViewTextWrapExecute(TObject *Sender);
	void __fastcall ActionBlobParseModeDetectExecute(TObject *Sender);
	void __fastcall ActionBlobParseUnzipExecute(TObject *Sender);
	void __fastcall ActionBlobParseContainerExecute(TObject *Sender);
	void __fastcall ActionBlobParseUnzip2Execute(TObject *Sender);
	void __fastcall TreeViewContainerChange(TObject *Sender, TTreeNode *Node);
	void __fastcall TreeViewContainerExpanded(TObject *Sender, TTreeNode *Node);
	void __fastcall TreeViewContainerCollapsed(TObject *Sender, TTreeNode *Node);
	void __fastcall ActionSaveConfigExecute(TObject *Sender);
	void __fastcall ActionSaveConfigSaveExecute(TObject *Sender);
	void __fastcall ActionBlobParseValueStorageExecute(TObject *Sender);
	void __fastcall ActionBlobParseValue1CExecute(TObject *Sender);
	void __fastcall TimerGarbageMemoryTimer(TObject *Sender);
	void __fastcall StringGridPhysicalViewGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign, Advobj::TVAlignment &VAlign);
	void __fastcall StringGridPhysicalViewGetDisplText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall PanelMessagesResize(TObject *Sender);
	void __fastcall ActionFindSupplierConfigsExecute(TObject *Sender);
	void __fastcall ActionSaveSupplierConfigExecute(TObject *Sender);
	void __fastcall ActionSaveDataFileExecute(TObject *Sender);
	void __fastcall ActionSaveBlobFileExecute(TObject *Sender);
	void __fastcall ActionSaveIndexFileExecute(TObject *Sender);
	void __fastcall ComboBoxCurrentIndexChange(TObject *Sender);
	void __fastcall ActionDumpIndexExecute(TObject *Sender);
	void __fastcall ActionExportTableToXMLExecute(TObject *Sender);
	void __fastcall ActionAboutExecute(TObject *Sender);
	void __fastcall ActionViewFileOffsetExecute(TObject *Sender);
	void __fastcall ActionShowGUIDsExecute(TObject *Sender);
	void __fastcall ActionOpenBaseNotMonopolyExecute(TObject *Sender);
	void __fastcall TabSheetPhysicalViewResize(TObject *Sender);
	void __fastcall ActionTestIndexesExecute(TObject *Sender);
	void __fastcall StringGridPhysicalViewCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall StringGridPhysicalViewGetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor);
	void __fastcall ListViewTablesChanging(TObject *Sender, TListItem *Item, TItemChange Change, bool &AllowChange);
	void __fastcall ActionEditTableExecute(TObject *Sender);
	void __fastcall ActionExportTableExecute(TObject *Sender);
	void __fastcall ActionImportTableExecute(TObject *Sender);
	void __fastcall ActionExportDataTablesExecute(TObject *Sender);
	void __fastcall ActionImportDataTablesExecute(TObject *Sender);
	void __fastcall FormControlEditLinkCommonGetEditorValue(TObject *Sender, TAdvStringGrid *Grid, UnicodeString &AValue);
	void __fastcall FormControlEditLinkCommonSetEditorFocus(TObject *Sender, TAdvStringGrid *Grid, TWinControl *AControl);
	void __fastcall StringGridPhysicalViewGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState, TBrush *ABrush,
          TFont *AFont);
	void __fastcall StringGridPhysicalViewCanInsertRow(TObject *Sender, int ARow, bool &CanInsert);
	void __fastcall StringGridPhysicalViewCanDeleteRow(TObject *Sender, int ARow, bool &CanDelete);
	void __fastcall ActionEditTableInsertExecute(TObject *Sender);
	void __fastcall ActionEditTableDeleteExecute(TObject *Sender);
	void __fastcall ActionSaveTableChangesExecute(TObject *Sender);
	void __fastcall ActionDiscardTableChangesExecute(TObject *Sender);
	void __fastcall AdvStringGridStatisticGetFormat(TObject *Sender, int ACol, TSortStyle &AStyle, UnicodeString &aPrefix, UnicodeString &aSuffix);
	void __fastcall AdvStringGridStatisticCanSort(TObject *Sender, int ACol, bool &DoSort);
	void __fastcall AdvStringGridStatisticGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign, Advobj::TVAlignment &VAlign);
	void __fastcall AdvStringGridStatisticGridHint(TObject *Sender, int ARow, int ACol, UnicodeString &hintstr);
	void __fastcall StringGridPhysicalViewEditCellDone(TObject *Sender, int ACol, int ARow);
	void __fastcall ActionNULLExecute(TObject *Sender);
	void __fastcall ActionRestoreExecute(TObject *Sender);
	void __fastcall ActionClearExecute(TObject *Sender);
	void __fastcall ButtonedEditCommonEditKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Open1CDFile(String FileName);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ActionFindLostObjectsExecute(TObject *Sender);
	void __fastcall ActionTestStreamFormatExecute(TObject *Sender);
	void __fastcall ActionCreateEmptyObjectExecute(TObject *Sender);
	void __fastcall ActionCreateObjectFromFileExecute(TObject *Sender);
	void __fastcall ActionCreateAndImportTableExecute(TObject *Sender);
	void __fastcall ActionEnableEditExecute(TObject *Sender);
	void __fastcall ActionTestListTablesExecute(TObject *Sender);
	void __fastcall ActionCreateTablesExecute(TObject *Sender);
	void __fastcall ActionReplaceTREFExecute(TObject *Sender);
	void __fastcall ActionBlobParseUsersUsrExecute(TObject *Sender);
	void __fastcall StringGridPhysicalViewCanSort(TObject *Sender, int ACol, bool &DoSort);
	void __fastcall ActionAssociateFileExecute(TObject *Sender);
	void __fastcall ActionXMLSaveBLOBToFileExecute(TObject *Sender);
	void __fastcall ActionXMLUnpackBLOBExecute(TObject *Sender);
	void __fastcall ActionDeleteTableExecute(TObject *Sender);
	void __fastcall ActionFindAndCreateLostTablesExecute(TObject *Sender);
	void __fastcall ActionDeleteWrongTablesExecute(TObject *Sender);
	void __fastcall ActionFindTableDoublesExecute(TObject *Sender);
	void __fastcall ActionFindAndSaveLostObjectsExecute(TObject *Sender);
	void __fastcall ActionCommandLineKeysExecute(TObject *Sender);
	void __fastcall ActionTestStreamFormatExternalExecute(TObject *Sender);
	void __fastcall StringGridPhysicalViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall StringGridPhysicalViewKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MaskEditCommonEdit_MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall ButtonedEditCommonEditRightButtonClick(TObject *Sender);
	void __fastcall ButtonedEditCommonEditChange(TObject *Sender);
	void __fastcall ActionWriteBlobToFileExecute(TObject *Sender);
	void __fastcall ActionReadBlobFromFileExecute(TObject *Sender);
	void __fastcall ActionBlobClearExecute(TObject *Sender);
	void __fastcall ActionBlobRestoreExecute(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall ActionSaveLastDepotConfigExecute(TObject *Sender);
	void __fastcall ActionSaveDepotConfigExecute(TObject *Sender);
	void __fastcall ActionRestoreDATAAllocationTableExecute(TObject *Sender);
	void __fastcall ActionSaveConfigExtensionExecute(TObject *Sender);
	void __fastcall ActionSaveConfigExtensionDbExecute(TObject *Sender);



private:	// User declarations
	Messager* mess; // регистратор сообщений
	T_1CD* base1CD; // база 1CD

	TMemoryStream* temp_stream; // поток для просмотра Blob-данных (преобразованный blob_stream)
	//TFileStream* temp_stream; // поток временного файла для просмотра Blob-данных
	wchar_t* temp_name; // имя временного файла для просмотра Blob-данных
	TMemoryStream* blob_stream; // Текущее содержание blob-поля (не преобразованное)

	char* tabrec; // буфер для одной записи текущей таблицы

	v8catalog* cat; // текущий контейнер поля blob;
	TableReadThread* tr_thread; // поток фонового чтения таблицы

	index* cur_index; // текущий индекс таблицы
	//unsigned int cur_row; // текущая строка таблицы

	String __fastcall PhysViewGetDisplText(int ACol, int ARow, bool EmptyNull = false, wchar_t Delimiter = 0, bool detailed = false);
	void __fastcall SetEditTableVisible();
	void __fastcall RefreshListOfTables();
	void __fastcall PopupEditMenu(int x = -1, int y = -1);
	void __fastcall validateNumber(String& str, int length, int precision, int& s, int& sl);
	void __fastcall AddDirectoryToContainer(String DirName, v8catalog* cat);

	int edit_numfield; // индекс текущего редактируемого поля в tab
	unsigned int edit_phys_numrecord; // физический номер текущей редактируемой записи в tab
	unsigned int edit_ARow; // номер текущей редактируемой строки StringGridPhysicalView
	bool edit_tab; // признак текущего редактирования tab

	bool edit_clear; // видимость кнопки очистки в режиме редактирования
	bool edit_null; // видимость кнопки NULL в режиме редактирования
	bool edit_restore; // видимость кнопки восстановления значения в режиме редактирования

	CommandParse* comm; // класс команд командной строки

	TCustomEdit* curedit;

public:		// User declarations
	table* tab; // текущая таблица
	int sorted_column; // текущий столбец сортировки
	bool reverse_sort; // признак обратной сортировки (по убыванию)

	__fastcall TFormMain(TComponent* Owner);
	__fastcall ~TFormMain();
	void __fastcall setBlobViewPropertiesVisible(bool _Visible);
	void __fastcall setContainerListVisible(bool _Visible);
	void __fastcall setBlobViewText(bool is_string, bool is_unicode = true);
	void __fastcall ParseBlob(int ARow);
	void __fastcall ReSort();
	unsigned int __fastcall get_phys_numrec(int ARow); // получить физический индекс записи по номеру строки по текущему индексу в текущей таблице
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------

class Messager : public MessageRegistrator
{
private:
	TListView* ListView;
	TStatusBar* StatusBar;
	String logfile;
	TFormatSettings FormatSettings;
public:
	__fastcall Messager(TListView* lv, TStatusBar* sb);
	void __fastcall setlogfile(String _logfile);
	virtual void __fastcall AddMessage(const String& message, const MessageState mstate, TStringList* param = NULL);
	virtual void __fastcall Status(const String& message);
};

#endif

