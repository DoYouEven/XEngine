#ifndef VECTOR_H
#define VECTOR_H
#endif
#include <cmath>
#include <iostream>

class Vector3
{
public:
	float x, y, z;


	//vector3d(float a, float b);

	Vector3(float a = 0.0f, float b= 0.0f, float c=0.0f)
	{
		x = a;
		y = b;
		z = c;
	}
	
	float dotProduct(const Vector3 &vec3)
	{
		return (x*vec3.x + y*vec3.y + z*vec3.z);
	}
	
	Vector3 crossProduct(const Vector3 &vec3)
	{
		return Vector3(y*vec3.z - z*vec3.y, z*vec3.x - x*vec3.z, x*vec3.y - y *vec3.x);
	}

	float magnitude()
	{
		return sqrt(x*x + y*y + z*z);
	}
	Vector3 normalize()
	{
		float _mag = magnitude();
		if (_mag != 0)
		{
			x /= _mag;
			y /= _mag;
			z /= _mag;
		}
		return *this;

	}

	void update(float a, float b, float c)

	{
		x = a;
		y = b;
		z = c;
	}
		
	
	void update(Vector3 vec3)
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;
	}

	void setX(float _x);
	void setY(float _x);
	void setZ(float _x);

	Vector3 operator+(const Vector3 &vec3)
	{
		return Vector3(vec3.x + x, vec3.y + y, vec3.z + z);
	}
	Vector3 operator-(const Vector3 &vec3)
	{
		
			return Vector3(x - vec3.x , y - vec3.y, z - vec3.z );
		
	}
	Vector3 operator/(float vec3)
	{
		{
			return Vector3(x/vec3, y/vec3, z/vec3);
		}
	}
	Vector3 operator*(float vec3)
	{
		return Vector3(x*vec3, y*vec3, z*vec3 );
	}
	Vector3 operator*(const Vector3 &vec3)
	{
		return Vector3(y*vec3.z - z*vec3.y, z*vec3.x - x*vec3.z, x*vec3.y - y *vec3.x);
	}
	Vector3 operator+=(const Vector3 &vec3)
	{
		return *this = (*this + vec3);
	}
	Vector3 operator-=(const Vector3 &vec3)
	{
		return *this = (*this - vec3);
	}
	Vector3 operator/=(float vec3)
	{
		return *this = (*this /vec3);
	}
	Vector3 operator*=(float vec3)
	{
		return *this = (*this * vec3);
	}
	//friend std::ostream& operator <<(std::ostream& out, const Vector3 &vec);
	bool operator == (const Vector3 vec3)

	{
		return (x == vec3.x && y == vec3.y && vec3.z == z);
	}
	bool operator != (const Vector3 vec3)
	{
		return !(x == vec3.x && y == vec3.y && vec3.z == z);
	}

	
};