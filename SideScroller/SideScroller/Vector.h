#ifndef VECTOR_H
#define VECTOR_H

#include <list>

class Vector
{
private:
	float _x;
	float _y;
public:
	Vector(float x, float y);
	Vector();
	
	//Get/Set
	float getX();
	float getY();
	void setX(float x);
	void setY(float y);

	//Functions
	float length();
	float distance(Vector& vec);

	//Operators
	Vector operator+(Vector& vec);
	Vector operator-(Vector& vec);
	Vector operator*(Vector& vec);
	Vector operator=(Vector& vec);
	Vector operator*(float& scalar);

	Vector addTo(Vector& vec);
	Vector divideBy(float factor);
	Vector scale(float scalar);

	static Vector vectorSum(std::list<Vector>* _list);
	static Vector normalize(Vector& vec);
	static float dot(Vector& vec1, Vector& vec2);

	//Debug
	void printVector();
};

#endif