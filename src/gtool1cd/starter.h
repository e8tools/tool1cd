#ifndef STARTERWINDOW_H
#define STARTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class StarterWindow;
}

class StarterWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit StarterWindow(QWidget *parent = 0);
	~StarterWindow();

	void openDatabase(const QString &filename);

private slots:

	void on_openNewButton_clicked();

private:
	Ui::StarterWindow *ui;
};

#endif // STARTERWINDOW_H
