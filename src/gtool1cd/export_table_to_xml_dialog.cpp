#include "export_table_to_xml_dialog.h"
#include "ui_export_table_to_xml_dialog.h"
#include <Table.h>
#include <QFileDialog>
#include <QMessageBox>

ExportTableToXmlDialog::ExportTableToXmlDialog(Table *table, QWidget *parent) :
    QDialog(parent),
    table(table),
    ui(new Ui::ExportTableToXmlDialog)
{
	ui->setupUi(this);
}

ExportTableToXmlDialog::~ExportTableToXmlDialog()
{
	delete ui;
}

void ExportTableToXmlDialog::show_with_file_dialog()
{
	QString exportFileName = QFileDialog::getSaveFileName(this,
	                             tr("Экспорт таблицы"),
	                             "", "*.xml");
	if (exportFileName.isNull()) {
		return;
	}

	ui->filePathEdit->setText(exportFileName);
	table->fill_records_index();
	show();
}

void ExportTableToXmlDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	if (!(ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)) {
		return;
	}

	table->export_to_xml(ui->filePathEdit->text().toStdString(),
	                     ui->exportBlobCheckBox->checkState() == Qt::Checked,
	                     ui->unpackBlobCheckBox->checkState() == Qt::Checked);

	QMessageBox::information(this, tr("Экспорт таблицы"), tr("Выгрузка завершена!"));
}
