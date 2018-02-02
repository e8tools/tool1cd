#ifndef LITTLELOGWINDOW_H
#define LITTLELOGWINDOW_H

#include <QMainWindow>
#include <MessageRegistration.h>

namespace Ui {
class LittleLogWindow;
}

class LittleLogWindow : public QMainWindow, public MessageRegistrator
{
	Q_OBJECT

public:
	explicit LittleLogWindow(QWidget *parent = 0);
	~LittleLogWindow();

	void addLogMessage(const QString &message);

	virtual void AddMessage(
	        const String &description,
	        const MessageState mstate,
	        const TStringList *param = nullptr) override
	{
		addLogMessage(QString(description.c_str()));
	}
	virtual void Status(const String& message) override
	{

	}

private slots:


private:
	Ui::LittleLogWindow *ui;
};

#endif // LITTLELOGWINDOW_H
