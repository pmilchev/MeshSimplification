#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

class Vector3
{
public:

	union
	{
		struct {
			float x, y, z;
		};
		float f[3];
	};

	Vector3();
	Vector3(const Vector3 & other);
	Vector3(float xx, float yy, float zz);

	Vector3& operator =  (const Vector3& other);
	float   operator *  (const Vector3& other) const;
	Vector3& operator ^= (const Vector3& other);
	Vector3  operator ^  (const Vector3& other);
	Vector3& operator += (const Vector3& other);
	Vector3  operator +  (const Vector3& other) const;
	Vector3& operator -= (const Vector3& other);
	Vector3  operator -  (const Vector3& other) const;
	Vector3& operator *= (float c);
	Vector3  operator *  (float c) const;
	Vector3& operator /= (float c);
	Vector3  operator /  (float c) const;
	float len() const;
	void normalize();
	bool operator != (const Vector3& other) const;
	bool operator == (const Vector3& other) const;
};

Vector3 operator*(float c, const Vector3 & v);

#endif  // __VECTOR_3_H__