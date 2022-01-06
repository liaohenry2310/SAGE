#pragma once

namespace Sage::Math
{
	struct Vector3
	{
		union
		{
			struct { float x, y, z; };
			std::array<float, 3> v;
		};

		//Constructors
		constexpr Vector3() noexcept : Vector3{ 0.0f } {}
		explicit constexpr Vector3(float f) noexcept : Vector3{ f, f, f } {}
		constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

		//Operators
		Vector3 operator-() const;
		Vector3 operator+(const Vector3& v) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3 operator*(float s) const;
		Vector3 operator/(float s) const;
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
		bool operator==(const Vector3& v);

		//Constants
		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;
	};
}