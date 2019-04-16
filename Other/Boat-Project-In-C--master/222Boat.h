#pragma once

#include <ostream>
#include <list>
#include <vector>
#include <string>
#include <string.h>
#include <time.h>

//class prototypes
/////////////////////////////////////////////////////////////
class Tank
{
	public:
		const float getCapacity();
		float getMeter();
		bool isEmpty();
		bool isFull();
		void drain(float);
		void refill();
		void fill(float);
	protected:
		Tank(float);
		~Tank() {}
		float capacity;
		float meter;
	private:
		Tank() {}
};

/////////////////////////////////////////////////////////////
class FuelTank : public Tank
{
	public:
		friend class Motor;
		FuelTank(float);
		~FuelTank() {}
		void refuel();
	private:
		FuelTank() : Tank(0) {}
};

/////////////////////////////////////////////////////////////
class WaterTank : public Tank
{
	//capacity percent based?
	//what is water tank? hold fresh water? used water?
	//will use it for maximum water boat can hold before capsizing

	public:
		WaterTank(float);
		~WaterTank() {}
		void flush();
	private:
		WaterTank() : Tank(0) { }
};

/////////////////////////////////////////////////////////////
class Motor
{
	public:
		enum Status { NOT_RUNNING = 0, RUNNING};
		Motor(int strokes, int hPowers);
		~Motor() {}
		void start();
		void stop();
		Status getStatus() { return status; }
		int getStrokes() { return numStrokes; }
		int getHorsePower() { return horsePower; }
	private:
		Motor() {}
		Status status;
		int numStrokes;
		int horsePower;
};

/////////////////////////////////////////////////////////////
class Boat;//forward declare

class Pump
{
	protected:
		Pump(float rate);
		Pump() : rate(0) {}
		virtual ~Pump() {}
		virtual void pump(Boat &) = 0; //pure virtual
		const float rate;
};

/////////////////////////////////////////////////////////////
class ElectricPump : public Pump
{
	public:
		ElectricPump();
		virtual ~ElectricPump() {}
		virtual void pump(Boat &);
};

/////////////////////////////////////////////////////////////
class ManualPump : public Pump
{
	public:
		ManualPump();

		virtual ~ManualPump() {}
		virtual void pump(Boat &);
};

/////////////////////////////////////////////////////////////
class CoastGuardBoat;
class Boat
{
	public:
		enum Damage { CAPSIZED = 0, NOMINAL};
		
		//damage functions
		void damageBoat(float leakRate);
		bool takeDamage();
		bool isDead();
		Damage getDamage();
		
		//water functions
		bool isLeaking();
		float leakingRate();
		float waterTakenIn();
		void pumpWater(float);
		bool pump();
		bool manualPump();

		//movement functions
		bool move(int knots);//boat cannot move if no people onboard
		void startMotor();
		void stopMotor();
		int getDistance();
		int getDistanceToDestination() { return destination - distanceTraveled; };
		void setDestination(int knots);//knots from starting shorline lets say
		bool canReachDestination(float target);
		bool canReachDestination();
		bool isAtdestination();
		void dock();
		bool canStart();
		float getMeter();

		//people functions
		void people(int);
		int getPeople();
		int getOpenSpots();
		
		//conveniance and logging 
		std::string toString();
		void SOS();
		friend std::ostream& operator<<(std::ostream&, Boat&);
		const std::list<std::string>& getLog();

	protected:
		friend class CoastGuardBoat;
		friend class ManualPump;
		Boat(int numstrokes, int hPower, float water_capacity,
			float fuel_capacity,
			int size, int speed, int people);

		~Boat() {}

		int getMaxSpeed();
		int getSize();

		WaterTank wTank; 
		FuelTank fTank; //gallons
		Motor motor;

		int destination;
		int distanceTraveled; //in knots away from shoreline
		int size;
		int numPeople;
		int peopleCapacity;
		int currentSpeed;
		int maxSpeed;
		float water_in;//inrate

		void log();
	private:

		typedef std::pair<int, CoastGuardBoat *> DistanceBoatPair;
		void queueSort(std::vector<DistanceBoatPair> &);
		bool sosSort(const DistanceBoatPair &first,const DistanceBoatPair& second);
		Damage damage;
		std::list<std::string> logList;
		Boat() : wTank(0), fTank(0), motor(0, 0) {}
};

/////////////////////////////////////////////////////////////
class SailBoat : public Boat
{
	public:

		//lowered is not in use
		//spinnaker usually only used when main sail is active
		enum Sail {
			SAIL_RAISED = 2, SAIL_LOWERED = 1,
			SPINNAKER_RAISED = 5, SPINNAKER_LOWERED = 3
		};
		SailBoat(int numstrokes, int horses, float water_capacity,
			float fuel_capacity,
			int size, int speed,
			int ppl, bool hasSpinnaker);

		~SailBoat() {}
		bool hasSpinnaker();
		void raiseMainSail();
		void raiseSpinnaker();
		void lowerMainSail();
		void lowerSpinnaker();
		void move(int knots);
		Sail getSailState();
	private:
		void updateSpeed();
		bool spinnaker;
		Sail sail;
};

/////////////////////////////////////////////////////////////
class PowerBoat : public Boat
{
	public:
		PowerBoat(int numstrokes, int hPower, float water_capacity,
			float fuel_capacity,
			int size, int speed, int ppl);

		~PowerBoat() {}
		void startMotor();
		void stopMotor();
		void move(int knots);
	private:
		Motor secondMotor;
};

/////////////////////////////////////////////////////////////
//let us say litoral is US 25-foot defender class boat(Response Boat)
//https://en.wikipedia.org/wiki/Defender-class_boat
//let us say opensea is US 47-foot motor lifeboat - MLB
//https://en.wikipedia.org/wiki/47-foot_Motor_Lifeboat

class CoastGuardBoat : public PowerBoat
{
	public:
		enum CoastGuardType { LITORAL = 2, OPENSEA }; //2 knots out maximum for litoral boats
		static std::list<CoastGuardBoat*> allCoastGuards;//rather use just a tree
		
		CoastGuardBoat(CoastGuardType);
		~CoastGuardBoat();
		bool rescue(Boat &);
	private:
		
};

