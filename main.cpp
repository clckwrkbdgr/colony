#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));

	QApplication a(argc, argv);
	MainWindow wnd;
	wnd.showMaximized();
	return a.exec();
}
