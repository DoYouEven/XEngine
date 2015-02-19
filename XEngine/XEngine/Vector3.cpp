#include "Vector3.h"

/*
Vector 3 Template
Generic Vector 3 functionality

Provides Arthematic operations accross vectors including dot and cross product.
*/
Vector3::Vector3()
{
	x = y = z = 0;
}
Vector3::Vector3(float a, float b, float c)
{
	x = a; y = b; z = c;

}

float Vector3::dotProduct(const Vector3 &vec3)
{
	return x*vec3.x + y*vec3.y + z*vec3.z;
}

Vector3 Vector3::crossProduct(const Vector3 &vec3)
{
	return Vector3();
}

float Vector3::magnitude()
{
	return sqrt(x*x + y*y + z*z);
}
void Vector3::normalize()
{
	float _mag = magnitude();
	if (_mag != 0)
	{
		x /= _mag;
		y /= _mag;
		z /= _mag;
	}

}

void Vector3::update(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}


void Vector3::update(Vector3 vec3)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}

void Vector3::setX(float _x)
{
	x = _x;
}
void Vector3::setY(float _y)
{
	y = _y;
}
void Vector3::setZ(float _z)
{
	z = _z;
}

Vector3 Vector3::operator+(const Vector3 &vec3)
{
	return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
}
Vector3 Vector3::operator-(const Vector3 &vec3)
{
	return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
}
Vector3 Vector3::operator/(float mag)
{
	if (mag == 0)
		return Vector3();
	return Vector3(x/mag, y/mag, z/mag);
}

Vector3 Vector3::operator*(float mag)
{
	return Vector3(x * mag, y * mag, z * mag);
}

Vector3 Vector3::operator+=(const Vector3 &vec3)
{
	x += vec3.x;
	y += vec3.y;
	z += vec3.z;
	return *this;
}
Vector3 Vector3:: operator-=(const Vector3 &vec3)
{
	x -= vec3.x;
	y -= vec3.y;
	z -= vec3.z;
	return *this;
}
Vector3 Vector3::operator/=(float mag)
{
	if (mag != 0)
	{
		x /=mag;
		y /= mag;
		z /= mag;
		return *this;

	}

}
Vector3 Vector3::operator*=(float mag)
{
	x *= mag;
	y *= mag;
	z *= mag;
	return *this;
}
Vector3 Vector3::operator*(const Vector3 &vec3)
{
	return Vector3(y*vec3.z - z*vec3.y,z*vec3.x - x*vec3.z, x*vec3.y - y *vec3.x);
}
bool Vector3::operator == (const Vector3 vec3)
{
	return (x == vec3.x && y == vec3.y && vec3.z == z);
}
bool Vector3::operator != (const Vector3 vec3)
{
	return !(x == vec3.x && y == vec3.y && vec3.z == z);
}

std::ostream& operator <<(std::ostream& out, const Vector3 &vec)
{
	return out << "Vector =[" << vec.x << " " << vec.y << " " << vec.z << "]\n";
	
}