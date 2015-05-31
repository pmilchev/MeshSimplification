#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <math.h>

class Vector
{
public:

	union
	{
		struct {
			float x, y, z;
		};
		float f[3];
	};

	Vector();
	Vector(const Vector & other);
	Vector(float xx, float yy, float zz);

	Vector& operator =  (const Vector& other);
	float   operator *  (const Vector& other) const;
	Vector& operator ^= (const Vector& other);
	Vector  operator ^  (const Vector& other);
	Vector& operator += (const Vector& other);
	Vector  operator +  (const Vector& other) const;
	Vector& operator -= (const Vector& other);
	Vector  operator -  (const Vector& other) const;
	Vector& operator *= (float c);
	Vector  operator *  (float c) const;
	Vector& operator /= (float c);
	Vector  operator /  (float c) const;
	float len() const;
	void normalize();
	bool operator != (const Vector& other) const;
	bool operator == (const Vector& other) const;
};

#endif  // __VECTOR_3_H__