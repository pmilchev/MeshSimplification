#include "vector3.h"

Vector::Vector() {}

Vector::Vector(const Vector& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector::Vector(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

Vector& Vector::operator = (const Vector& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

float Vector::operator * (const Vector& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector& Vector::operator ^= (const Vector& other)
{
	float tmp[3] = {
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	};
	for (unsigned i = 0; i < 3; i++) f[i] = tmp[i];
	return *this;
}

Vector Vector::operator ^ (const Vector& other)
{
	Vector tmp(*this);
	return tmp ^= other;
}

Vector& Vector::operator += (const Vector& other)
{
	for (unsigned i = 0; i < 3; i++) f[i] += other.f[i];
	return *this;
}

Vector Vector::operator + (const Vector& other) const
{
	Vector tmp(*this);
	return tmp += other;
}

Vector& Vector::operator -= (const Vector& other)
{
	for (unsigned i = 0; i < 3; i++) f[i] -= other.f[i];
	return *this;
}

Vector Vector::operator - (const Vector& other) const
{
	Vector tmp(*this);
	return tmp -= other;
}

Vector& Vector::operator *= (float c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

Vector Vector::operator* (float c) const
{
	Vector tmp(*this);
	return tmp *= c;
}

Vector& Vector::operator /= (float c)
{
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

Vector Vector::operator / (float c) const
{
	Vector tmp(*this);
	return tmp /= c;
}

float Vector::len() const
{
	return sqrtf(x*x + y*y + z*z);
}

void Vector::normalize()
{
	operator*(1 / len());
}

bool Vector::operator != (const Vector& other) const{
	for (unsigned i = 0; i < 3; i++)
		if (f[i] != other.f[i]) return true;
	return false;
}

bool Vector::operator == (const Vector& other) const{
	return !(*this != other);
}