#pragma once
#include "Vector3.h"

namespace Sage::Math
{
	struct Quaternion;

	struct Matrix4
	{
		union
		{
			struct // row+column subscript
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			std::array<float, 16> v;
		};

		const static Matrix4 Identity;

		constexpr Matrix4 operator*(float s) const
		{
			return
			{
				_11 * s, _12 * s, _13 * s, _14 * s,
				_21 * s, _22 * s, _23 * s, _24 * s,
				_31 * s, _32 * s, _33 * s, _34 * s,
				_41 * s, _42 * s, _43 * s, _44 * s,
			};
		}

		constexpr Matrix4 operator*(const Matrix4& rhs) const
		{
			return
			{
				(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
				(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
				(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
				(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

				(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
				(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
				(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
				(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

				(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
				(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
				(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
				(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

				(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
				(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
				(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
				(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44)
			};
		}

		static Matrix4 Translation(const Vector3& d)
		{
			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				d.x, d.y, d.z, 1.0f
			};
		}

		static Matrix4 Scaling(const Vector3& s)
		{
			return {
				s.x, 0.0f, 0.0f, 0.0f,
				0.0f, s.y, 0.0f, 0.0f,
				0.0f, 0.0f, s.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4 RotationX(float rad)
		{
			const float c = cos(rad);
			const float s = sin(rad);
			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, c, s, 0.0f,
				0.0f, -s, c, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4 RotationY(float rad)
		{
			const float c = cos(rad);
			const float s = sin(rad);
			return {
				c, 0.0f, -s, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				s, 0.0f, c, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4 RotationZ(float rad)
		{
			const float c = cos(rad);
			const float s = sin(rad);
			return {
				c, s, 0.0f, 0.0f,
				-s, c, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		static Matrix4 RotationAxis(const Vector3& axis, float radian);
		static Matrix4 RotationQuaternion(const Quaternion& q);
	};
}


