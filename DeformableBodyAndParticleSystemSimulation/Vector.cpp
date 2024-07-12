#include <math.h>

#include "Vector.h"

Vec3::Vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vec3::Vec3(const float& a, const float& b, const float& c)
{
	x = a;
	y = b;
	z = c;
}

void Vec3::setZero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

float Vec3::length()
{
	return sqrtf(x * x + y * y + z * z);
}

Vec3 Vec3::normalize()
{
	float length = sqrtf(x * x + y * y + z * z);
	return Vec3(x / length, y / length, z / length);
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Vec3& Vec3::operator*=(const float& c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

Vec3& Vec3::operator/=(const float& c)
{
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vec3 operator*(const float& c, const Vec3& v)
{
	return Vec3(c * v.x, c * v.y, c * v.z);
}

Vec3 operator*(const Vec3& v, const float& c)
{
	return Vec3(c * v.x, c * v.y, c * v.z);
}

Vec3 operator/(const Vec3& v, const float& c)
{
	return Vec3(v.x / c, v.y / c, v.z / c);
}

float dotProduct(const Vec3& lhs, const Vec3& rhs)
{
	return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
}

Vec3 crossProduct(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}
