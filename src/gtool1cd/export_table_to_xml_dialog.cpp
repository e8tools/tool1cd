/*
    GTool1CD provides GUI front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of GTool1CD.

    GTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
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
