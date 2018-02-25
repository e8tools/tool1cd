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
	setWindowTitle(QString::fromStdString(db->getfilename()));
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

	db->save_configsave(targetFileName.toStdString());
}

void ConfigurationsWindow::on_saveDbConfigButton_clicked()
{
	QString targetFileName = QFileDialog::getSaveFileName(this, tr("Сохранение конфигурации"), "", "*.cf");
	if (targetFileName.isNull()) {
		return;
	}

	db->save_config(targetFileName.toStdString());
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
