#ifndef EXPORT_TABLE_TO_XML_DIALOG_H
#define EXPORT_TABLE_TO_XML_DIALOG_H

#include <QDialog>
#include <QAbstractButton>

class Table;

namespace Ui {
class ExportTableToXmlDialog;
}

class ExportTableToXmlDialog : public QDialog
{
	Q_OBJECT

public:
	ExportTableToXmlDialog(Table *table, QWidget *parent = 0);
	~ExportTableToXmlDialog();

	void show_with_file_dialog();

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);

private:
	Ui::ExportTableToXmlDialog *ui;
	Table *table;
};

#endif // EXPORT_TABLE_TO_XML_DIALOG_H
