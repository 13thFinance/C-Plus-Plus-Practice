#include <iostream>
#include "VectorGraphics.h"



using namespace std;

const int WIDTH = 1000, HEIGHT = 800;

//tester function
void spin(VectorGraphics&);
int displayMenu();

int main(int argc, char *argv[])
{
	bool done = false;

	cout << "Vector Drawing software Version 0.0.1\n" << endl;
	VectorGraphics vg(1000, 800);

	int option = 0;
	int index = 0;
	bool choice = false;
	int r, g, b;
	double scaleX, scaleY;

	while (!done)
	{
		option = displayMenu();

		switch (option)
		{
		case 0:
			vg.display();
			break;
		case 1:
			double mag, theta;
			cin >> mag >> theta;
			vg.addVector(mag, theta);
			break;
		case 2:
			double Fx, Fy;
			cin >> Fx >> Fy;
			vg.addVector(vector<double>(Fx, Fy));
			break;
		case 3:
			cout << "Vector: ";
			cin >> index;

			vg.removeVector(index);
			break;
		case 4:
			vg.printVectors();
			break;
		case 5:
			cout << "\n0: off\n1: on\n";
			cin >> choice;
			vg.displayAxis(choice);
			break;
		case 6:
			cout << "\n0: off\n1: on\n";
			cin >> choice;
			vg.displayGrid(choice);
			break;
		case 7:
			cout << "\nRGB values must be given: ";
			cin >> r >> g >> b;
			vg.setBackgroundColor(r , g, b);
			break;
		case 8:
			cout << "\nRGB values must be given: ";
			cin >> r >> g >> b;
			vg.setAxisColor(r, g, b);
			break;
		case 9:
			cout << "Notes: reference is top left\n";
			cin >> Fx >> Fy;
			vg.setAxis(Fx, Fy);
			break;
		case 10:
			cin >> scaleX >> scaleY;
			vg.setScale(scaleX, scaleY);
			break;
		case 11:
			cout << "\nRGB values must be given: ";
			cin >> r >> g >> b;
			vg.setGridColor(r, g, b);
			break;
		case -1:
			done = true;
		default:
			break;
		}
	}

	return EXIT_SUCCESS;
}

void spin(VectorGraphics &vg)
{
	for (double i = 0; i < 360; i++)
	{
		vg.addVector(10, i);
		vg.draw();
		vg.wait(3);
		vg.removeVector(4);
	}
}

//displays menu for interfacing with VectorGraphics object
int displayMenu()
{
	bool valid = true;
	int option = 0;
	do
	{
		cout << "Menu: \n"
			<< "0: Draw Vectors\n"
			<< "1: Add vector(magnitude, theta)\n"
			<< "2: Add vector(Fx, Fy)\n"
			<< "3: Remove vector\n"
			<< "4: Display vectors\n"
			<< "5: Options\n"
			<< "-1: Exit program\n";

		cin >> option;

		if (option == 5)
		{
			cout << "\n\nOptions:\n"
				<< "5:  Display axis\n"
				<< "6:  Display grid\n"
				<< "7:  Set Background Color\n"
				<< "8:  Set axis color\n"
				<< "9:  Set axis/origin\n"
				<< "10: Set Scale\n"
				<< "11: Set grid color\n";

			cin >> option;
		}

		if (option < -1 || option > 11)
		{
			cout << "Invalid Input!\n";
			valid = false;
		}
		else
		{
			valid = true;
		}
	} while (!valid);

	return option;
}
