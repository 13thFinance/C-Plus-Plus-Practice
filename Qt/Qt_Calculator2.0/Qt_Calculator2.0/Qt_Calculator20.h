#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt_Calculator20.h"

class Qt_Calculator20 : public QMainWindow
{
	Q_OBJECT

public:
	Qt_Calculator20(QWidget *parent = Q_NULLPTR);

private:
	Ui::Qt_Calculator20Class ui;
private slots:
	void evaluateExpression();
	void clearInput();
};
