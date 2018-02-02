#include "starter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StarterWindow w;
	w.show();

	return a.exec();
}
