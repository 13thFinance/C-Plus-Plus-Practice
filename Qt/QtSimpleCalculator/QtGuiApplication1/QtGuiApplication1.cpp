#include "QtGuiApplication1.h"
#include <QtCore>
#include <string.h>

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//edit pushButton text atribute
	std::string calculateName = "Calculate";
	ui.calculate->setText(calculateName.c_str());//cant take string for whatever reason
	connect(ui.calculate, SIGNAL(clicked()), this, SLOT(calculate()));
}

void QtGuiApplication1::calculate()
{
	const int INPUTS = 3;
	//will be used to store operands 
	std::string inputs[INPUTS];

	//get std::string in from input1 text
	QString qstring = ui.input1->toPlainText();
	inputs[0] = qstring.toStdString();

	//get std::string from operand text
	qstring = ui.operand->toPlainText();
	inputs[1] = qstring.toStdString();

	//get std::string from input2 text
	qstring = ui.input2->toPlainText();
	inputs[2] = qstring.toStdString();

	//sanitation
	bool validInput = true;

	validInput = isNumber(inputs[0]);//test input 1

	if (validInput)
	{
		validInput = isNumber(inputs[2]);//test input 2
	}

	if (validInput)
	{
		if (inputs[1].compare("+") == 0 || inputs[1].compare("-") == 0 || inputs[1].compare("*") == 0 || inputs[1].compare("/") == 0)
		{

		}
		else
		{
			validInput = false;
		}
	}

	if (!validInput)
	{
		ui.result->setText("Invalid input!");
		return;
	}
	else
	{
		//calculations
		double in1 = std::stod(inputs[0]);
		double in2 = std::stod(inputs[2]);

		char operand = inputs[1].at(0);

		double result = 0;

		switch (operand)
		{
		case '+':
			result = in1 + in2;
			break;
		case '-':
			result = in1 - in2;
			break;
		case '*':
			result = in1 * in2;
			break;
		case '/':
			result = in1 / in2;
			break;
		default:
			break;
		}

		//write out the result
		ui.result->setText(std::to_string(result).c_str());
	}
	
	return;
}

//returns true if a string is a full number
//returns false if the string is not a full number
bool QtGuiApplication1::isNumber(std::string string)
{
	int dotCounter = 0;

	for (char c : string)
	{
		if (c == '.')
		{
			dotCounter++;

			if (dotCounter >= 2)
			{
				return false;
			}

			continue;
		}

		if (!(c >= '0' && c <= '9'))
		{
			return false;
		}
	}

	return true;
}