#ifndef CONTAINER_FORM_H
#define CONTAINER_FORM_H

#include <QWidget>

namespace Ui {
class ContainerForm;
}

class ContainerForm : public QWidget
{
	Q_OBJECT

public:
	explicit ContainerForm(QWidget *parent = 0);
	~ContainerForm();

	void open(const QString &filename);

private:
	Ui::ContainerForm *ui;
};

#endif // CONTAINER_FORM_H
