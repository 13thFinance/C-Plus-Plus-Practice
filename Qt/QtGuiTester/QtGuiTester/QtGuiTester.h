#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiTester.h"

class QtGuiTester : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiTester(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiTesterClass ui;
private slots:
	void exit();
};
