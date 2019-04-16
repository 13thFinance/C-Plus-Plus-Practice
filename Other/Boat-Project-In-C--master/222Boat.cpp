#pragma once
#include "222Boat.h"

//Tank//////////////////////////////
Tank::Tank(float maxCapacity)
{
	capacity = maxCapacity;
	meter = capacity;
}

const float Tank::getCapacity()
{
	return capacity;
}

float Tank::getMeter()
{
	return meter;
}

bool Tank::isEmpty()
{
	return (meter > 0) ? false : true;
}

bool Tank::isFull()
{
	return (meter == capacity);
}

void Tank::refill()
{
	meter = capacity;
}

void Tank::fill(float inFill)
{
	if (meter + inFill >= capacity)
	{
		meter = capacity;
	}
	else
	{
		meter += inFill;
	}
}

void Tank::drain(float drain)
{
	(meter > drain) ? meter -= drain : meter = 0.0;
}

//FuelTank///////////////////////////
FuelTank::FuelTank(float maxCapacity) : Tank(maxCapacity) {};

void FuelTank::refuel()
{
	refill();
}

//WaterTank/////////////////////////
WaterTank::WaterTank(float maxCapacity) : Tank(maxCapacity) {};

void WaterTank::flush()
{
	meter = 0;
}

//Motor/////////////////////////////

Motor::Motor(int strokes, int hPowers)
{
	numStrokes = strokes;
	horsePower = hPowers;
	status = NOT_RUNNING;
}

void Motor::start()
{
	status = RUNNING;
}

void Motor::stop()
{
	status = NOT_RUNNING;
}

//pump//////////////////////////////////
Pump::Pump(float inRate) : rate(inRate){};

//electric pump////////////////////////
ElectricPump::ElectricPump()
{
}

void ElectricPump::pump(Boat& boat)
{
	boat.pumpWater(boat.waterTakenIn());
}

//manual pump
ManualPump::ManualPump()
{
}

void ManualPump::pump(Boat & boat)
{
	//lets assume people use buckets to get rid of water
	//, for science
	boat.pumpWater((float)(boat.getPeople() / 2));
}

//Boat/////////////////////////////////

Boat::Boat(int numstrokes, int hPower, float water_capacity,
	float fuel_capacity, int Size, int speed, int people)
	: wTank(water_capacity), fTank(fuel_capacity), motor(numstrokes, hPower)
{	
	srand(time(0));
	size = Size;
	maxSpeed = speed;
	peopleCapacity = people;
	water_in = (rand() % (int)(water_capacity / 10));
	wTank.flush();
	numPeople = 0;
	distanceTraveled = 0;
	currentSpeed = 0;
	destination = 0;
	damage = NOMINAL;
	log();
}

void Boat::damageBoat(float leakRate)
{
	water_in += leakRate;
	log();
}

Boat::Damage Boat::getDamage()
{
	return damage;
}

bool Boat::pump()
{
	//lets say electric pump takes gas to power generator
	if (!fTank.isEmpty() && wTank.getMeter() > 0)
	{
		//lets simplify .1 gal of gas per 10 gal of water per tick
		wTank.drain(10);
		fTank.drain(.1);
		return true;
	}

	return false;
}

bool Boat::manualPump()
{
	//lets say electric pump takes gas to power generator
	if (wTank.getMeter() > 0)
	{
		//lets say for every 2 people we can drain 2 gallons per tick
		wTank.drain(numPeople / 2);
		return true;
	}

	return false;
}

bool Boat::takeDamage()
{
	wTank.fill(water_in);

	if (wTank.isFull() || fTank.isEmpty())
	{
		damage = CAPSIZED;
		stopMotor();
		log();
		return true;
	}

	log();
	return false;
}

bool Boat::isDead()
{
	if (damage == CAPSIZED || fTank.isEmpty())
		return true;
	return false;
}

bool Boat::move(int knots)
{
	takeDamage();//automatically take damage at least once

	//see bottom
	float gasConsumptionPerKnot = ((motor.getStrokes() == 2 ? .57 : .5) 
									* (float)motor.getHorsePower() / 6.1) * (float)0.868976;//1mile/hr = .868976 knots

	for (int i = 0; i < knots; i++)
	{
		if (isDead() || motor.getStatus() == Motor::NOT_RUNNING)
		{
			log();
			return false;
		}
		
		if (distanceTraveled == destination)
		{
			//we reached destination
			logList.push_back("DESTINATION REACHED");
			log();
			break;
		}

		distanceTraveled++;

		//see bottom
		fTank.drain(gasConsumptionPerKnot);//drain in gal/knot

		if (i > 0)
		{
			takeDamage();//one tick of damage per knot
		}
	}
	
	log();
	return true;
}

void Boat::startMotor()
{
	if (numPeople > 0)
	{
		motor.start();

		currentSpeed = maxSpeed;
	}

	log();
}

void Boat::stopMotor()
{
	motor.stop();

	currentSpeed = 0;

	log();
}

void Boat::people(int passengers)
{
	//no overloading
	if (passengers >= getOpenSpots())
	{
		numPeople = peopleCapacity;
	}
	else
	{
		numPeople += passengers;
	}
	
	log();
}

void Boat::pumpWater(float waterPumped)
{
	wTank.drain(waterPumped);
	log();
}

bool Boat::isLeaking()
{
	return (water_in > 0) ? true : false;
}

float Boat::leakingRate()
{
	return water_in;
}

float Boat::waterTakenIn()
{
	return wTank.getMeter();
}

std::string Boat::toString()
{
	std::string boatString("\nBoat Log #" + std::to_string(logList.size() + 1) + (std::string)"\n------------\n");
	boatString.append("Passengers: " + std::to_string(numPeople) + '\n');
	boatString.append("Distance from Shoreline: " + std::to_string(distanceTraveled) + (std::string)" knots\n");

	bool moving = motor.getStatus() == Motor::RUNNING;

	boatString.append("Moving: " + std::to_string((moving ? true : false)) + '\n');

	if (moving)
	{
		boatString.append("Speed: " + std::to_string(currentSpeed) + (std::string)" knots\n");
	}

	if (!fTank.isEmpty() && wTank.isEmpty())
	{
		boatString.append("Status: Nominal\n");
		return boatString;
	}
	if (fTank.isEmpty())
	{
		boatString.append("Status: No Fuel\n");
	}
	if (!wTank.isEmpty())
	{
		boatString.append("Status: Taking in Water\n");
	}

	return boatString;
}

int Boat::getDistance()
{
	return distanceTraveled;
}

void Boat::setDestination(int knots)
{
	destination = knots;
}

bool Boat::canReachDestination(float target)
{
	Boat temp(*this);

	temp.move(target - distanceTraveled);
	temp.move(distanceTraveled);//make sure you can come back

	if (temp.isDead())
	{
		return false;
	}

	return true;
}

bool Boat::canReachDestination()
{
	Boat temp(*this);

	temp.move(destination - distanceTraveled);
	temp.move(distanceTraveled);//make sure you can come back

	if (temp.isDead())
	{
		return false;
	}

	return true;
}

bool Boat::isAtdestination()
{
	if (getDistanceToDestination() == 0)
	{
		return true;
	}
	return false;
}

void Boat::dock()
{
	distanceTraveled = 0;
	wTank.flush();
	fTank.refill();
	stopMotor();
	water_in = 0; //fix boat
	damage = NOMINAL;
	logList.push_back((std::string)"\n\nBOAT DOCKED!\n\n");
	log();
}

int Boat::getOpenSpots()
{
	return (peopleCapacity - numPeople);
}

bool Boat::sosSort(const DistanceBoatPair &first,const DistanceBoatPair& second)
{
	//vestige of me trying to use binarySort from std lib, my computer couldnt find it
	return ((int)first.first < (int)second.first);
}

void Boat::queueSort(std::vector<DistanceBoatPair> & array)
{
	for (unsigned int i = 0; i < array.size(); i++)
	{
		for (unsigned int j = i; j < array.size(); j++)
		{
			if (!sosSort(array[i], array[j]))
			{
				DistanceBoatPair temp(array[i]);
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

void Boat::SOS()////////////////////////////////
{
	if (numPeople > 0 && (CoastGuardBoat::allCoastGuards).size() > 0)
	{
		std::vector<DistanceBoatPair> queue;

		for (CoastGuardBoat * rescueBoat : CoastGuardBoat::allCoastGuards)
		{
			if (rescueBoat->getOpenSpots() > 0)
			{
				queue.push_back(DistanceBoatPair(distanceTraveled - rescueBoat->distanceTraveled, rescueBoat));//<distance from boat, rescueboat>
			}
		}

		queueSort(queue);

		for (DistanceBoatPair rescueBoatPair : queue)
		{
			CoastGuardBoat * rescueBoatPtr = rescueBoatPair.second;

			if (rescueBoatPtr->canReachDestination(distanceTraveled - rescueBoatPtr->distanceTraveled) && 
				rescueBoatPtr->rescue(*this))
			{
				
			}
		}
	}

	damage = CAPSIZED;
	logList.push_back("Boat Rescued/Abandoned\n\nEND LOG!!!\n");
}

bool Boat::canStart()
{
	if (fTank.getMeter() > 0 && damage != CAPSIZED && numPeople > 0)
	{
		return true;
	}

	return false;
}

float Boat::getMeter()
{
	return fTank.getMeter();
}

int Boat::getPeople()
{
	return numPeople;
}

int Boat::getMaxSpeed()
{
	return maxSpeed;
}

int Boat::getSize()
{
	return size;
}

std::ostream & operator<<(std::ostream &os, Boat &boat)
{
	os << "\nSize: " << boat.getSize() << std::endl <<
		"Passengers: " << boat.getPeople() << std::endl <<
		"Maximum Capacity: " << boat.peopleCapacity << std::endl <<
		"Maximum Speed: " << boat.maxSpeed << std::endl <<
		"Status: " << (boat.isDead() ? "DEAD": "OK") << std::endl <<
		"Is Leaking: " << (boat.isLeaking() ? "TRUE" : "FALSE") << std::endl;

	if (boat.isLeaking())
	{
		os << "Leak Rate: " << boat.leakingRate() << std::endl;
	}

	os << "Water capacity before capsizing: " << boat.wTank.getCapacity() << std::endl;

	if (boat.isLeaking())
	{
		os << "Current water taken in: " << boat.wTank.getMeter() << std::endl;
	}

	os << "Is Moving: " << ((boat.motor.getStatus() == 0) ? "NOT RUNNING" : "RUNNING") << std::endl;
	os << "Distance traveled: " << boat.getDistance() << std::endl;

	return os;
}

void Boat::log()
{
	logList.push_back(toString());
}

const std::list<std::string> & Boat::getLog()
{
	return logList;
}

//Sailboat/////////////////////////////////////////////
SailBoat::SailBoat(int numstrokes, int horses, float water_capacity, float fuel_capacity, 
					int size, int speed, int ppl, bool hasSpinnaker) :
	Boat(numstrokes, horses, water_capacity, fuel_capacity, size, (speed > 5 ? 5 : speed), ppl)
{
	spinnaker = hasSpinnaker;
	sail = SAIL_LOWERED;
}

bool SailBoat::hasSpinnaker()
{
	return spinnaker;
}

void SailBoat::updateSpeed()
{
	if (spinnaker)
	{
		currentSpeed = sail;
	}
	else
	{
		switch (sail)
		{
			case SailBoat::SAIL_RAISED:
			case SailBoat::SAIL_LOWERED:
				currentSpeed = sail;
				break;
			default:
				break;
		}
	}
	
}

void SailBoat::raiseMainSail()
{
	sail = SAIL_RAISED;
	motor.start();
	updateSpeed();
	log();
}

void SailBoat::raiseSpinnaker()
{
	if (spinnaker)
	{
		sail = SPINNAKER_RAISED;
		updateSpeed();
		log();
	}
}

void SailBoat::lowerMainSail()
{
	sail = SAIL_LOWERED;
	motor.stop();
	updateSpeed();
	log();
}

void SailBoat::lowerSpinnaker()
{
	if (spinnaker)
	{
		sail = SPINNAKER_LOWERED;
		updateSpeed();
		log();
	}
}

void SailBoat::move(int knots)
{
	takeDamage();//automatically take damage at least once

	for (int i = 0; i < knots; i++)
	{
		if (isDead())
			break;

		distanceTraveled++;

		if (i > 0)
		{
			takeDamage();//one tick of damage per knot
		}
	}

	log();
}

SailBoat::Sail SailBoat::getSailState()
{
	return sail;
}

//PowerBoat////////////////////////////////////////////
PowerBoat::PowerBoat(int numstrokes, int hPower, float water_capacity, 
	float fuel_capacity, int size, int speed, int ppl) : 
	Boat(numstrokes, hPower, water_capacity, fuel_capacity, size, speed, ppl),
	secondMotor(numstrokes, hPower)
{

}

void PowerBoat::startMotor()
{
	secondMotor.start();
	Boat::startMotor();
}

void PowerBoat::stopMotor()
{
	secondMotor.stop();
	Boat::stopMotor();
}

void PowerBoat::move(int knots)
{
	takeDamage();//automatically take damage at least once

				 //see bottom
	float gasConsumptionPerKnot = ((motor.getStrokes() == 2 ? .57 : .5)
		* (float)motor.getHorsePower() / 6.1) * 0.868976;//1mile/hr = .868976 knots

	for (int i = 0; i < knots; i++)
	{
		if (isDead())
			break;

		distanceTraveled++;

		//see bottom
		fTank.drain(gasConsumptionPerKnot * 2);//drain in gal/knot * engines

		if (i > 0)
		{
			takeDamage();//one tick of damage per knot
		}
	}

	log();
}

//CoastGuardBoat///////////////////////////////////////

CoastGuardBoat::CoastGuardBoat(CoastGuardType type) : PowerBoat((type == LITORAL) ?
	4, 225, 100, 105, 30, 46, 10 :
	4, 435, 100, 373, 48, 25, 34)
{
	allCoastGuards.push_back(this);
}

CoastGuardBoat::~CoastGuardBoat()
{
	allCoastGuards.remove(this);
}

bool CoastGuardBoat::rescue(Boat & boat)
{
	//open spots in coastguard
	int openSpots = getOpenSpots();


	if (openSpots == 0)
	{
		return false;
	}

	//assuming coast guard will never overload
	if (openSpots > boat.numPeople)
	{
		numPeople += boat.numPeople;
		boat.numPeople = 0;
	}
	else
	{
		numPeople += openSpots;
		boat.numPeople -= openSpots;
	}

	logList.push_back(std::string("\n\nBoat Rescued!\n\n"));

	boat.log();
	log();
	return true;
}

std::list<CoastGuardBoat *> CoastGuardBoat::allCoastGuards = {};//static initializer!!!!


/*
fuel economy calculations

Gallon per Hour = (specific fuel consumption x HP)/Fuel Specific Weight
lets say we use gas and Peak HP:
fuel specific weight = 6.1 lb/gal

four stroke: specific fuel consumption = ~.5 lb
two stroke: specific fuel consumption = ~.57 lb

we could have used table boat specific fuel consumption:
+------------------------------------------+----------+----------------+-----------+
|                  Engine                  | @ 1K RPM | @ Peak Torque  | @ Peak HP |
+------------------------------------------+----------+----------------+-----------+
| 4-stroke (low compression, carburetor)   | 0.62     | 0.47           | 0.52      |
| 4-stroke (high compression, carburetor)  | 0.60     | 0.42           | 0.47      |
| 4-stroke (high compression, closed loop) | 0.52     | 0.35           | 0.42      |
| 4-stroke (supercharged, carburetor)      | 0.75     | 0.50           | 0.55      |
| 4-stroke (turbocharged, closed loop)     | 0.57     | 0.45           | 0.50      |
| 2-stroke (low compression, carburetor)   | 0.85     | 0.55           | 0.60      |
| 2-stroke (high compression, carburetor)  | 0.80     | 0.50           | 0.55      |
| Diesel 4-stroke                          | 0.35     | 0.25           | 0.30      |
| Diesel 2-stroke                          | 0.40     | 0.29           | 0.34      |
+------------------------------------------+----------+----------------+-----------+

but thats too complicated for this program maybe
we could also divide by hp/10 for quick calculation with +- 20%...
resource: https://www.boatingmag.com/calculating-fuel-consumption
*/