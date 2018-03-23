#ifndef SKOBKATEXTWINDOW_H
#define SKOBKATEXTWINDOW_H

#include <QDialog>

namespace Ui {
class SkobkaTextWindow;
}

class SkobkaTextWindow : public QDialog
{
	Q_OBJECT

public:
	explicit SkobkaTextWindow(QWidget *parent = 0);
	~SkobkaTextWindow();

	void setText(const QString &text, const QString &title = QString());

private:
	Ui::SkobkaTextWindow *ui;
};

#endif // SKOBKATEXTWINDOW_H
