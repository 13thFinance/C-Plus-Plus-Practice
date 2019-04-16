#include "QtGuiTester.h"

QtGuiTester::QtGuiTester(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(exit()));
}

void QtGuiTester::exit()
{
	QApplication::exit();
}