#include "Vector.h"
#include <cmath>
#include <iostream>

//Constructor
Vector::Vector(float x, float y)
{
	_x = x;
	_y = y;
}

Vector::Vector()
{
	_x = 0;
	_y = 0;
}

//Get/Set
float Vector::getX()
{
	return _x;
}

float Vector::getY()
{
	return _y;
}

void Vector::setX(float x)
{
	_x = x;
}

void Vector::setY(float y)
{
	_y = y;
}

//Functions
float Vector::dot(Vector& vec1, Vector& vec2)
{
	return (vec1.getX()*vec2.getX()) + (vec1.getY()*vec2.getY());
}

float Vector::length()
{
	return (float)sqrtf((_x*_x) + (_y*_y));
}

float Vector::distance(Vector& vec)
{
	float x_diff = vec.getX() - _x;
	float y_diff = vec.getY() - _y;
	return (float)sqrtf(((x_diff)*(y_diff)) + ((x_diff)*(y_diff)));
}

Vector Vector::scale(float scalar)
{
	float xx = _x * scalar;
	float yy = _y * scalar;

	if (abs(xx) < 0.00001)
		xx = 0;
	if (abs(yy) < 0.00001)
		yy = 0;

	setX(xx);
	setY(yy);

	return (*this);
}


Vector Vector::normalize(Vector& vec)
{
	double len = vec.length();
	Vector vector(vec.getX()/len, vec.getY()/len);

	return vector;
}

Vector Vector::vectorSum(std::list<Vector>* _list)
{
	Vector v = Vector(0, 0);
	auto end = _list->end();
	for (auto it = _list->begin(); it != end; it++)
	{
		v = it->addTo(v);
	}
	return v;
}


//Operators
Vector Vector::operator+(Vector& vec)
{
	Vector vector(this->getX() + vec.getX(), this->getY() + vec.getY());

	return vector;
}

Vector Vector::operator-(Vector& vec)
{
	Vector vector(this->getX() - vec.getX(), this->getY() - vec.getY());

	return vector;
}

Vector Vector::operator*(Vector& vec)
{
	Vector vector(this->getX() * vec.getX(), this->getY() * vec.getY());

	return vector;
}

Vector Vector::operator=(Vector& vec)
{
	setX(vec.getX());
	setY(vec.getY());

	return *this;
}

Vector Vector::operator*(float& scalar)
{
	Vector vector(this->getX() * scalar, this->getY() * scalar);

	return vector;
}

Vector Vector::addTo(Vector& vec)
{
	Vector vector(this->getX() + vec.getX(), this->getY() + vec.getY());

	return vector;
}

Vector Vector::divideBy(float factor)
{
	if (factor != 0)
	{
		float xx = _x / factor;
		float yy = _y / factor;

		if (abs(xx) < 0.00001)
			xx = 0;
		if (abs(yy) < 0.00001)
			yy = 0;

		setX(xx);
		setY(yy);

		return (*this);
	}
}

//Debug
void Vector::printVector()
{
	std::cout << "Vector: (" << _x << ", " << _y << ")" << std::endl;
}