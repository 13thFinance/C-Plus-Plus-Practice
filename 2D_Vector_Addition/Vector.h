#pragma once

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <string>
#include <string.h>
#include <ostream>

/*
class representing 2d vectors
*/
class Vector
{
public:

	//number of default components
	static const int COMPONENT_SIZE = 2;

	Vector();
	Vector(double magnitude, double theta); //polar cordinate based vector
	Vector(std::vector<double> &components); //<x,y> component based vector
	~Vector();

	//getters
	double getMagnitude() { return magnitude; };
	double getTheta() { return theta; };
	double getFx() { return Fx; };
	double getFy() { return Fy; };
	int getQ() { return q; };

	std::string toString();
	friend std::ostream & operator<<(std::ostream &out, Vector v);
	Vector operator+(const Vector&);

private:
	double magnitude;
	double theta;
	double Fx, Fy;
	int q;

	double degreesToRadians(double degrees);
	double radiansToDegrees(double radians);
};

//0 vector
inline Vector::Vector()
{
	magnitude = theta = 0;
	q = 1;
	//fill components with 0
	Fx = Fy = 0;
}

//vector magnitude, theta, x and y values get stored
//theta is using +x reference(standard reference)
inline Vector::Vector(double magnitude, double theta)
{
	this->magnitude = magnitude;
	this->theta = theta;

	theta = std::fmod(theta, 360);

	//assuming 2d vector
	//x-line used as the reference angle
	if (theta >= 0 && theta <= 90)//angle lies in q1, x and y component are positive
	{
		q = 1;
		Fx = magnitude * cos(degreesToRadians(theta)); //xval
		Fy = magnitude * sin(degreesToRadians(theta)); //yval
	}
	else if (theta > 90 && theta <= 180)//angle lies in q2
	{
		q = 2;
		double tempAngle = 180 - theta; //give inside angle in reference to -x line

		Fx = (magnitude * cos(degreesToRadians(theta))); //xval is negative
		Fy = magnitude * sin(degreesToRadians(theta)); //yval is positive
	}
	else if (theta > 180 && theta < 270) //angle lies in q3, both components are negative
	{
		q = 3;
		double tempAngle = theta - 180;

		Fx = (magnitude * cos(degreesToRadians(theta))); //xval negative
		Fy = (magnitude * sin(degreesToRadians(theta))); //yval negative
	}
	else //angle lies in q4, yval is negative
	{
		q = 4;
		double tempAngle = 360 - theta;
		
		Fx = magnitude * cos(degreesToRadians(theta));
		Fy = (magnitude * sin(degreesToRadians(theta)));
	}
}

//takes in a vector with x and y components
//calculates magnitude and theta from +x line and stores everything
//input is a vector of size 2, with [0] being x magnitude, and [1] being y magnitude
//throws an exception if invalid
inline Vector::Vector(std::vector<double>& components)
{
	try
	{
		Fx = components[0];
		Fy = components[1];
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Error at vector instantion!\n Adress: " << this << std::endl;
		getchar();
	}

	magnitude = sqrt((Fx * Fx) + (Fy * Fy));

	if (Fx == 0)
	{
		if (Fy > 0)
		{
			theta = 90;
			q = 4; //for reversing in VectorGraphics.h
		}
		else if (Fy < 0)
		{
			theta = 270;
			//????? TODO ///////////////////////////////////////////////////////
		}
		else
		{
			theta = 0;
		}
	}
	else
	{
		theta = radiansToDegrees(atan(abs(Fy) / abs(Fx))); //q1
		q = 1;
		if (Fx < 0 && Fy > 0) //q2
		{
			q = 2;
			theta += (90 - theta) * 2;
		}
		else if (Fx < 0 && Fy < 0) //q3
		{
			q = 3;
			theta += 180;
		}
		else if (Fx > 0 && Fy < 0) //q4
		{
			q = 4;
			theta -= 2 * theta;
		}
	}
}

inline Vector::~Vector()
{
}

//returns a string version of the vector
inline std::string Vector::toString()
{
	std::string vString = "";
	vString.append("Magnitude: [" + std::to_string(magnitude) + "]\n");
	vString.append("Theta: [" + std::to_string(theta)+ "]\n");
	vString.append("Components: <" + std::to_string(Fx) + ", "+ std::to_string(Fy) + ">\n");
	
	return vString;
}

inline std::ostream & operator<<(std::ostream &out, Vector v)
{
	out << v.toString();
	return out;
}

//adds two vectors together and returns the resultant vector
inline Vector Vector::operator+(const Vector &other)
{
	return Vector(std::vector<double>{this->Fx + other.Fx, this->Fy + other.Fy});
}

inline double Vector::degreesToRadians(double degrees)
{
	return degrees * M_PI / 180;
}

inline double Vector::radiansToDegrees(double radians)
{
	return (180 / M_PI) * radians;
}
