#pragma once

namespace Sage::Math
{
	struct Vector2
	{
		union
		{
			struct { float x, y; };
			std::array<float, 2> v;
		};

		//Constructors
		constexpr Vector2() noexcept : Vector2{ 0.0f } {}
		explicit constexpr Vector2(float f) noexcept : Vector2{ f, f} {}
		constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

		//Operators
		Vector2 operator+(const Vector2& v) const
		{
			return { x + v.x, y + v.y };
		}

		Vector2 operator-() const
		{
			return { -x, -y };
		}

		Vector2 operator-(const Vector2& v) const
		{
			return { x - v.x, y - v.y };
		}

		Vector2 operator*(float s) const
		{
			return { x * s, y * s };
		}

		Vector2 operator/(float s) const
		{
			return { x / s, y / s };
		}

		//Constants
		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 XAxis;
		const static Vector2 YAxis;
	};
}