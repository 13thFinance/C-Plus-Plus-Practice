/*
Programmer: Felix Ferreira
ID: 011001325

Notes:
solution space very broad
assuming you dont want trig and physical simulations
boats move accross a line.
WaterTank is a factor for capsizing and not one to hold other things(ie drinking water)
time not accounted for, adds unnecesary complexity for scope

...style.exe not found
*/

#include <iostream>
#include "222Boat.h"
#include <sstream>
#include <initializer_list>

using namespace std;

#ifdef _WIN32
	#include <windows.h>
#endif // _WIN32


//convenience
void header(string);
void endTest();
void shouldFail(bool);//pretty colors!!!(I got bored)...shouldve beefed up my implementation probably
void shouldSucceed(bool);//also makes it easier to identify errors
#define SHOULD_BE_TRUE ? "TRUE" : "FALSE"
#define SHOULD_BE_FALSE ? "FALSE" : "TRUE"
#define SHOULD_FAIL ? "FAIL!" : "SUCCESS!"
#define SHOULD_SUCCEED ? "SUCCESS!" : "FAIL!"

//tank tester, showcasing template
template <class tank>
void tankTester(initializer_list<tank> tankList)
{
	int count = 1;

	for (auto t : tankList)
	{
		cout << "Tank Test: " << count++ << "   :::::::::::::::::::::::::" << endl;
		cout << "Capacity: " << t.getCapacity() << endl
			<< "Meter: " << t.getMeter() << endl
			<< "IsEmpty: " << (t.isEmpty() SHOULD_BE_TRUE) << endl
			<< "IsFull: " << (t.isFull() SHOULD_BE_TRUE) << endl;
		cout << "Testing drain(100)..." << endl;
		t.drain(100);
		cout << "Meter: " << t.getMeter() << endl;
		cout << "Testing refill..." << endl;
		t.refill();
		cout << "Meter: " << t.getMeter() << endl;
		cout << "IsFull: " << (t.isFull() SHOULD_BE_TRUE) << endl;
		cout << "Testing full drain..." << endl;
		t.drain(t.getCapacity());
		cout << "Meter: " << t.getMeter() << endl;
		cout << "IsEmpty: " << (t.isEmpty() SHOULD_BE_TRUE) << endl;
		cout << "Testing fill(100): " << endl;
		t.fill(100);
		cout << "Meter: " << t.getMeter() << endl << endl;
	}
}

int main()
{
	//colors
	#ifdef _WIN32
		std::cout << "WINDOWS DETECTED! USING PRETTY COLORS!\n\n";
	#endif // _WIN32


	//tanks tester//////////////////////////////////////////////////
	//rip
	////////////////////////////////////////////////////////////////
	

	//fuel tank tester//////////////////////////////////////////////
	header("FUEL TANK TEST");

	tankTester({FuelTank(50.0), FuelTank(200.0), FuelTank(350.5)});

	{
		cout << "\nTesting refuel...\n";
		FuelTank temp(100.00);
		temp.drain(99.99);
		temp.refuel();

		shouldSucceed(temp.isFull());
	}
	endTest();
	////////////////////////////////////////////////////////////////

	//water tank tester/////////////////////////////////////////////
	header("WATER TANK TESTER");
	tankTester({ WaterTank(50.0), WaterTank(200.0), WaterTank(350.5) });

	{
		cout << "\nTesting refill...\n";
		WaterTank temp(100.00);
		temp.refill();
		temp.flush();
		shouldSucceed(!temp.isFull() && temp.getMeter() == 0);
	}
	endTest();
	////////////////////////////////////////////////////////////////

	//motor tester//////////////////////////////////////////////////
	header("MOTOR TESTER");

	{
		vector<Motor> motorArray = { Motor(4, 10), Motor(4, 200), Motor(2, 150), Motor(2, 200)};

		int test = 1;

		for (Motor temp : motorArray)
		{
			cout << "Motor Test: " << test << "   :::::::::::::::::::::::::\n";
			cout << "HP: " << (temp.getHorsePower()) << endl;
			cout << "Strokes: " << temp.getStrokes() << endl;
			cout << "Status: " << (temp.getStatus() == Motor::NOT_RUNNING ? "NOT_RUNNING" : "RUNNING") << endl;

			cout << "Testing start..." << endl;
			temp.start();
			shouldSucceed(temp.getStatus() == Motor::RUNNING);
			cout << "Testing stop..." << endl;
			temp.stop();
			shouldSucceed(temp.getStatus() == Motor::NOT_RUNNING);
		}
	}

	endTest();
	////////////////////////////////////////////////////////////////


	//boat tester///////////////////////////////////////////////////
	header("BOAT TESTER");

	{
		//run basic boat tests
		SailBoat SS_Junk(2, 200, 100, 500, 250, 10, 7, false);
		SailBoat SS_Streamline(2, 200, 100, 100, 20, 4, 6, true);
		PowerBoat pBoat(4, 300, 100, 250, 15, 10, 10);
		PowerBoat pBoat2(2, 300, 100.0, 150, 15, 30, 20);
		CoastGuardBoat USCG_defenderClass(CoastGuardBoat::LITORAL);
		CoastGuardBoat USCG_MLBoat(CoastGuardBoat::OPENSEA);

		vector<Boat*> myBoats;
		myBoats.push_back(&SS_Junk);
		myBoats.push_back(&SS_Streamline);
		myBoats.push_back(&pBoat);
		myBoats.push_back(&pBoat2);
		myBoats.push_back(&USCG_defenderClass);
		myBoats.push_back(&USCG_MLBoat);

		int test = 1;

		//tests base class functionality
		header("testing base class functionality");
		for (Boat * boatPtr : myBoats)
		{
			cout << "\n\nBoat Test " << test++ << "     :::::::::::::::::::::::::" << endl;
			cout << "Testing motor, no people on boat, should fail: ";
			shouldFail(boatPtr->canStart());
			boatPtr->people(2);
			cout << "Testing motors, boat boarded: ";
			shouldSucceed(boatPtr->canStart());
			cout << "Motor Started: ";
			boatPtr->startMotor();
			shouldSucceed(boatPtr->move(0));

			cout << "Setting destination(10 knots test)...\n";
			boatPtr->setDestination(10);
			cout << "Current destination distance: " << boatPtr->getDistanceToDestination() << endl;
			cout << "Checking if at destination: " << (!boatPtr->isAtdestination() SHOULD_BE_FALSE) << endl;
			bool reachTest = boatPtr->canReachDestination();
			cout << "Checking can reach destination: " << (reachTest SHOULD_BE_TRUE) << endl;//this should match
			cout << "Attempting to reach location..." << endl;
			while (!boatPtr->isAtdestination())
			{
				boatPtr->move(1); //tick

				if (boatPtr->isDead())
				{
					break;
				}
			}
			
			cout << "Did boat reach destination: " << (boatPtr->isAtdestination() SHOULD_BE_TRUE) << endl; //this////////////////////match
			cout << "Did Boat capsize/die: " << (boatPtr->isDead() SHOULD_BE_TRUE) << endl;
			cout << "Can reach destination function: ";
			shouldSucceed(boatPtr->isDead() != reachTest);
			cout << "Testing Dock()...\n";
			cout << *boatPtr;
			boatPtr->dock();
			cout << (*boatPtr);
			cout << "\nResetting boats through Dock(), should fail: ";//shouldnt be able to move since motor is stopped when docked
			shouldFail(boatPtr->move(1));

			cout << "Testing people(100) loading: ";
			boatPtr->people(100);
			shouldSucceed(boatPtr->getOpenSpots() == 0);
			cout << "Number of current passengers: " << boatPtr->getPeople() << endl;
		}

		//testing sailboats
		header("\n\nTesting SailBoats");

		cout << "Testing hasSpinakker(), should fail: ";
		shouldFail(SS_Junk.hasSpinnaker());
		cout << "Testing sail functions for SS_JUNK...\n";
		cout << "Is sail lowered: ";
		shouldSucceed((SS_Junk.getSailState() == SailBoat::SAIL_LOWERED));
		cout << "Raising main sail: ";
		SS_Junk.raiseMainSail();
		shouldSucceed((SS_Junk.getSailState() == SailBoat::SAIL_RAISED));
		cout << "Raising spinnaker, should fail: ";
		SS_Junk.raiseSpinnaker();
		shouldFail(SS_Junk.getSailState() == SailBoat::SPINNAKER_RAISED);
		cout << "Lowering mainSail: ";
		SS_Junk.lowerMainSail();
		shouldSucceed(SS_Junk.getSailState() == SailBoat::SAIL_LOWERED);

		cout << "Testing Sailboats dont use gas: ";
		SS_Junk.raiseMainSail();
		float meter = SS_Junk.getMeter();
		SS_Junk.move(1);
		shouldSucceed(meter == SS_Junk.getMeter());
		SS_Junk.dock();//reset

		//test powerBoats functions
		header("\n\nTesting PowerBoats");
		meter = pBoat.getMeter();
		pBoat.startMotor();
		pBoat.move(1);
		cout << "Powerboat should use gas: ";
		shouldSucceed(meter != pBoat.getMeter());
		cout << "CoastGuards should use gas: ";
		meter = USCG_defenderClass.getMeter();
		USCG_defenderClass.move(1);
		shouldSucceed(meter != USCG_defenderClass.getMeter());
		endTest();

		//testing sos
		cout << "Testing SOS:\n";
		//make trashboat to save later
		SailBoat SS_Trash(2, 200, 100, 500, 250, 10, 7, false);
		SS_Trash.damageBoat(10);//trash build quality tm
		SS_Trash.people(5);//load 5 people to move
		SS_Trash.move(3);

		//make an coastguard with no people loaded to save our trash boat
		CoastGuardBoat myCoastGuardBoat(CoastGuardBoat::LITORAL);
		myCoastGuardBoat.people(2);
		SS_Trash.SOS();

		//boat should be saved
		shouldSucceed(SS_Trash.getPeople() == 0);

		list<string> list = SS_Trash.getLog();

		cout << "Printing SOS boat log:\n";
		for (string s : list)
		{
			cout << s;
		}

		cout << "\n\nPrinting Coastguard Log:\n";
		list = myCoastGuardBoat.getLog();
		for (string s : list)
		{
			cout << s;
		}
	}

	endTest();
	////////////////////////////////////////////////////////////////

	std::system("pause");
	return 0;
}

void header(string header)
{
	cout << header << endl;
	string temp(header.size(), '-');
	cout << temp << endl;
}

void endTest()
{
	cout << "END TEST\n\n-----------------------------------------\n";
}

void shouldSucceed(bool statement)
{
#ifdef _WIN32
	
	/*
	bit 0 - foreground blue
	bit 1 - foreground green
	bit 2 - foreground red
	bit 3 - foreground intensity

	bit 4 - background blue
	bit 5 - background green
	bit 6 - background red
	bit 7 - background intensity
	*/

	//intense green (=
	const WORD green = 0x0A; //0000 1010
	//intense red (=
	const WORD red = 0x0C; //0000 1100

	

	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// remember how user started
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	if (statement == true)
	{
		SetConsoleTextAttribute(hstdout, green);
	}
	else
	{
		SetConsoleTextAttribute(hstdout, red);
	}


	cout << (statement SHOULD_SUCCEED) << endl;

	FlushConsoleInputBuffer(hstdin);

	//give user their defaults back
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);

	return;
#endif // _WIN32

	//if other system
	cout << (statement SHOULD_SUCCEED) << endl;
}

void shouldFail(bool statement)
{
#ifdef _WIN32

	//intense red (=
	const WORD red = 0x0C; //0000 1100
	//intense green (=
	const WORD green = 0x0A; //0000 1010

	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// remember how user started
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	if (statement == true)
	{
		SetConsoleTextAttribute(hstdout, red);
	}
	else
	{
		SetConsoleTextAttribute(hstdout, green);
	}

	cout << (statement SHOULD_SUCCEED) << endl;

	FlushConsoleInputBuffer(hstdin);

	//give user their defaults back
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);

	return;
#endif // _WIN32

	//if other system
	cout << (statement SHOULD_SUCCEED) << endl;
}