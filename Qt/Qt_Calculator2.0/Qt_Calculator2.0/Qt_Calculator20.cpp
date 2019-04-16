#include "Qt_Calculator20.h"
#include <string.h>
#include <string>

Qt_Calculator20::Qt_Calculator20(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.result->setPlainText("0"); //set default text to zero
	
	connect(ui.evaluate, SIGNAL(clicked()), this, SLOT(evaluateExpression()));//connect button click to evaluate expression
	connect(ui.clear, SIGNAL(clicked()), this, SLOT(clearInput()));
}

void Qt_Calculator20::evaluateExpression()
{
	//ui.result->setPlainText("Enter Registered!");

	//ewww, what is this, java?
	std::string expression = ui.expressionInput->toPlainText().toStdString();

	//break it into 3 parts
	std::string::size_type next; //time to break up the string into 3 parts

	double operand1 = 0, operand2 = 0;
	std::string operator_ = " ";

	try {
		operand1 = std::stod(expression, &next);//get first operand
		operator_ = expression.substr(next).at(0);//get first character from broken string
		operand2 = std::stod(expression.substr(next + 1)); //get second operand
	}
	catch (std::invalid_argument invalid)
	{
		ui.result->setPlainText("Check Input!");
		return;
	}


	if (operator_.compare("+") != 0 && operator_.compare("-") != 0 && operator_.compare("*") != 0 && operator_.compare("/") != 0)
	{
		ui.result->setPlainText("Check Operand!");
		return;
	}

	//calculations
	char operator_char = operator_.at(0);

	double result = 0;

	switch (operator_char)
	{
	case '+':
		result = operand1 + operand2;
		break;
	case '-':
		result = operand1 - operand2;
		break;
	case '*':
		result = operand1 * operand2;
		break;
	case '/':
		result = operand1 / operand2;
		break;
	default:
		break;
	}

	//write out the result
	ui.result->setPlainText(std::to_string(result).c_str());
}

void Qt_Calculator20::clearInput()
{
	ui.expressionInput->setPlainText("");
	return;
}
