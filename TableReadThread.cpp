//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TableReadThread.h"

//#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TableReadThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

TMultiReadExclusiveWriteSynchronizer* tr_syn = NULL;

//---------------------------------------------------------------------------
__fastcall TableReadThread::TableReadThread(bool CreateSuspended, TFormMain* _FormMain)
	: TThread(CreateSuspended)
{
	FormMain = _FormMain;
}

//---------------------------------------------------------------------------
void __fastcall TableReadThread::Execute()
{
	table* tab = FormMain->tab;

	if(!tab) return;

	unsigned int* recordsindex = tab->recordsindex; // массив индексов записей по номеру (только не пустые записи)
	bool& recordsindex_complete = tab->recordsindex_complete; // признак заполнености recordsindex
	unsigned int& numrecords_review = tab->numrecords_review; // количество просмотренных записей всего в поиске не пустых
	unsigned int& numrecords_found = tab->numrecords_found; // количество найденных непустых записей (текущий размер recordsindex)


	unsigned int numrecords = tab->get_phys_numrecords();
	unsigned int recordlen = tab->get_recordlen();
	char* rec = new char[recordlen];

	if(!recordsindex)
	{
		recordsindex = new unsigned int[numrecords];
		tr_syn->BeginWrite();
		tab->recordsindex = recordsindex;
		tr_syn->EndWrite();
	}

	for(; numrecords_review < numrecords; numrecords_review++)
	{
		if(Terminated)
		{
			delete[] rec;
			return;
		}
		tab->getrecord(numrecords_review, rec);
		if(rec[0]) continue;

		tr_syn->BeginWrite();
		recordsindex[numrecords_found++] = numrecords_review;
		tr_syn->EndWrite();
		if(numrecords_found % 30000 == 0) Synchronize((TThreadMethod)&SetLengthTable);

	}
	delete[] rec;
	recordsindex_complete = true;
	Synchronize((TThreadMethod)&SetLengthTable);
}

//---------------------------------------------------------------------------
void __fastcall TableReadThread::SetLengthTable()
{
	bool sel = false;
	if(FormMain->StringGridPhysicalView->RowCount == 1) sel = true;
	FormMain->StringGridPhysicalView->RowCount = FormMain->tab->numrecords_found + 1 + FormMain->tab->get_added_numrecords();
	FormMain->tab->set_log_numrecords(FormMain->tab->numrecords_found);
	if(sel) FormMain->StringGridPhysicalViewSelectCell(FormMain->StringGridPhysicalView, 0, 1, sel);

	if(FormMain->StringGridPhysicalView->RowCount > 2)
	{
		FormMain->StringGridPhysicalView->SortSettings->Column = FormMain->sorted_column;
		FormMain->StringGridPhysicalView->SortSettings->Direction = FormMain->reverse_sort ? sdDescending : sdAscending;
	}
	else FormMain->StringGridPhysicalView->SortSettings->Column = -1;

}

//---------------------------------------------------------------------------


