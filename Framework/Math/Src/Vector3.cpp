#include "Precompiled.h"
#include "Vector3.h"

using namespace Sage::Math;

Vector3 Vector3::operator+(const Vector3& v) const
{
	return { x + v.x, y + v.y, z + v.z };
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator-() const
{
	return { -x, -y, -z };
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return { x - v.x, y - v.y, z - v.z };
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator*(float s) const
{
	return { x * s, y * s, z * s };
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 Vector3::operator/(float s) const
{
	if (s == 0.0f)
	{
		return *this;
	}

	return { this->x / s, this->y / s, this->z / s };
}

Vector3& Vector3::operator/=(float s)
{
	if (s == 0.0f)
	{
		return *this;
	}

	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator==(const Vector3& v)
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

