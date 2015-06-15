#include "vector3.h"

Vector3::Vector3() {}

Vector3::Vector3(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3::Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

Vector3& Vector3::operator = (const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

float Vector3::operator * (const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3& Vector3::operator ^= (const Vector3& other)
{
	float tmp[3] = {
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	};
	for (unsigned i = 0; i < 3; i++) f[i] = tmp[i];
	return *this;
}

Vector3 Vector3::operator ^ (const Vector3& other)
{
	Vector3 tmp(*this);
	return tmp ^= other;
}

Vector3& Vector3::operator += (const Vector3& other)
{
	for (unsigned i = 0; i < 3; i++) f[i] += other.f[i];
	return *this;
}

Vector3 Vector3::operator + (const Vector3& other) const
{
	Vector3 tmp(*this);
	return tmp += other;
}

Vector3& Vector3::operator -= (const Vector3& other)
{
	for (unsigned i = 0; i < 3; i++) f[i] -= other.f[i];
	return *this;
}

Vector3 Vector3::operator - (const Vector3& other) const
{
	Vector3 tmp(*this);
	return tmp -= other;
}

Vector3& Vector3::operator *= (float c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

Vector3 Vector3::operator* (float c) const
{
	Vector3 tmp(*this);
	return tmp *= c;
}

Vector3& Vector3::operator /= (float c)
{
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

Vector3 Vector3::operator / (float c) const
{
	Vector3 tmp(*this);
	return tmp /= c;
}

float Vector3::len() const
{
	return sqrtf(x*x + y*y + z*z);
}

void Vector3::normalize()
{
	operator*(1 / len());
}

bool Vector3::operator != (const Vector3& other) const{
	for (unsigned i = 0; i < 3; i++)
		if (f[i] != other.f[i]) return true;
	return false;
}

bool Vector3::operator == (const Vector3& other) const{
	return !(*this != other);
}

Vector3 operator*(float c, const Vector3 & v)
{
	return v * c;
}