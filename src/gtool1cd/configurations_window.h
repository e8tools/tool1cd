#ifndef CONFIGURATIONS_WINDOW_H
#define CONFIGURATIONS_WINDOW_H

#include <QMainWindow>

class T_1CD;

namespace Ui {
class ConfigurationsWindow;
}

class ConfigurationsWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ConfigurationsWindow(T_1CD *db, QWidget *parent = 0);
	~ConfigurationsWindow();

private slots:

	void on_saveConfigButton_clicked();

	void on_saveDbConfigButton_clicked();

	void on_saveVendorConfigButton_clicked();

private:
	Ui::ConfigurationsWindow *ui;
	T_1CD *db;
};

#endif // CONFIGURATIONS_WINDOW_H
