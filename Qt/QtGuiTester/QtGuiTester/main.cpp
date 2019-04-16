#include "QtGuiTester.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiTester w;
	w.show();
	return a.exec();
}
