#include "starter.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "cache.h"
#include <QDebug>
#include <MessageRegistration.h>
#include <QCommandLineParser>

extern Registrator msreg_g;


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QIcon mainIcon(":/Images/MainIcon");
	a.setWindowIcon(mainIcon);

	StarterWindow w;
	w.setWindowIcon(mainIcon);
	w.setCache(new Cache());
	w.show();

	QCommandLineParser cli_parser;
	cli_parser.addPositionalArgument("filepath", QCommandLineParser::tr("Путь к файлу БД"));
	cli_parser.process(a);

	if (!cli_parser.positionalArguments().empty()) {
		for (QString &filepath_from_param : cli_parser.positionalArguments()) {
			w.openDatabase(filepath_from_param);
		}
	}

	return a.exec();
}
