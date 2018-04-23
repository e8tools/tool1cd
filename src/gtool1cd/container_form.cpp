#include "container_form.h"
#include "ui_container_form.h"
#include <QFileInfo>
#include <TFileStream.hpp>

ContainerForm::ContainerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContainerForm)
{
	ui->setupUi(this);
}

ContainerForm::~ContainerForm()
{
	delete ui;
}

void ContainerForm::open(const QString &filename)
{
	QFileInfo finfo(filename);
	boost::filesystem::path filepath(filename.toStdWString());
	auto stream = new TFileStream(filepath, fmOpenRead);
	ui->widget->setStream(stream, finfo.baseName());
	setWindowTitle(filename);
}
