#ifndef VECTOR_H
#define VECTOR_H

class Vec3
{
public:
	Vec3();
	Vec3(const float& a, const float& b, const float& c);
	void setZero();

	float length();

	Vec3 normalize();

	Vec3& operator+=(const Vec3& rhs);
	Vec3& operator-=(const Vec3& rhs);
	Vec3& operator*=(const float& c);
	Vec3& operator/=(const float& c);

	float x;
	float y;
	float z;
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
Vec3 operator*(const float& c, const Vec3& v);
Vec3 operator*(const Vec3& v, const float& c);
Vec3 operator/(const Vec3& v, const float& c);

float dotProduct(const Vec3& lhs, const Vec3& rhs);
Vec3 crossProduct(const Vec3& lhs, const Vec3& rhs);

#endif
