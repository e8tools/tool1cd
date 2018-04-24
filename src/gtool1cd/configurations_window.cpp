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
#include "configurations_window.h"
#include "ui_configurations_window.h"
#include <Class_1CD.h>
#include <QFileDialog>
#include "models/vendor_configurations_table_model.h"
#include <QMessageBox>

ConfigurationsWindow::ConfigurationsWindow(T_1CD *db, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigurationsWindow),
    db(db)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdWString(db->get_filepath().wstring()));
	ui->vendorsTable->setModel(new VendorConfigurationsTableModel(db));
}

ConfigurationsWindow::~ConfigurationsWindow()
{
	delete ui;
}

void ConfigurationsWindow::on_saveConfigButton_clicked()
{
	QString targetFileName = QFileDialog::getSaveFileName(this, tr("Сохранение конфигурации"), "", "*.cf");
	if (targetFileName.isNull()) {
		return;
	}

	db->save_configsave(boost::filesystem::path(targetFileName.toStdWString()));
}

void ConfigurationsWindow::on_saveDbConfigButton_clicked()
{
	QString targetFileName = QFileDialog::getSaveFileName(this, tr("Сохранение конфигурации"), "", "*.cf");
	if (targetFileName.isNull()) {
		return;
	}

	db->save_config(boost::filesystem::path(targetFileName.toStdWString()));
}

void ConfigurationsWindow::on_saveVendorConfigButton_clicked()
{
	auto model = static_cast<VendorConfigurationsTableModel*>(ui->vendorsTable->model());
	auto index = ui->vendorsTable->currentIndex();
	if (!index.isValid()) {
		return;
	}

	QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение конфигурации"), "", "*.cf");
	if (filename.isNull()) {
		return;
	}

	try {

		model->saveSupplierConfigToFile(index, filename);

	} catch (...) {
		QMessageBox::warning(this, tr("Сохранение"), tr("Ошибка сохранения конфигурации"));
	}
}
