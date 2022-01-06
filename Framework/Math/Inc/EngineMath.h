#pragma once

#include "Common.h"

//Linear Algrebra
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"

//Geometry
#include "AABB.h"
#include "OBB.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"

//Util
#include "Constants.h"
#include "Random.h"

namespace Sage::Math
{
	template <class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template <class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	constexpr float Abs(float value)
	{
		return value >= 0.0f ? value : -value;
	}

	constexpr float Sqr(float value)
	{
		return value * value;
	}

	constexpr float Dot(const Vector3& v0, const Vector3& v1)
	{
		return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
	}

	constexpr float MagnitudeSqr(const Vector2& v)
	{
		return (v.x * v.x) + (v.y * v.y);
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	inline float Magnitude(const Vector2& v)
	{
		return sqrt(MagnitudeSqr(v));
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrt(MagnitudeSqr(v));
	}

	constexpr float DistanceSqr(const Vector2& v0, const Vector2& v1)
	{
		return ((v0.x - v1.x) * (v0.x - v1.x)) + ((v0.y - v1.y) * (v0.y - v1.y));
	}

	constexpr float DistanceSqr(const Vector3& v0, const Vector3& v1)
	{
		return ((v0.x - v1.x) * (v0.x - v1.x)) + ((v0.y - v1.y) * (v0.y - v1.y)) + ((v0.z - v1.z) * (v0.z - v1.z));
	}

	inline float Distance(const Vector2& v0, const Vector2& v1)
	{
		return sqrt(DistanceSqr(v0, v1));
	}

	inline float Distance(const Vector3& v0, const Vector3& v1)
	{
		return sqrt(DistanceSqr(v0, v1));
	}

	inline Vector2 Normalize(const Vector2& v)
	{
		float magnitude = Magnitude(v);

		if (magnitude == 0.0f)
		{
			return v;
		}

		return v / magnitude;
	}

	inline Vector3 Normalize(const Vector3& v)
	{
		float magnitude = Magnitude(v);

		if (magnitude == 0.0f)
		{
			return v;
		}

		return v / magnitude;
	}

	constexpr Vector3 Cross(const Vector3& v0, const Vector3& v1)
	{
		return { (v0.y * v1.z) - (v0.z * v1.y), (v0.z * v1.x) - (v0.x * v1.z), (v0.x * v1.y) - (v0.y * v1.x) };
	}

	constexpr Matrix4 Transpose(const Matrix4& m)
	{
		return {
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		};
	}

	constexpr float Determinant(const Matrix4& m)
	{
		float det = 0.0f;
		det += (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
		det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
		det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
		det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
		return det;
	}

	constexpr Matrix4 Adjoint(const Matrix4& m)
	{
		return
		{
			+(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			+(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

			-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			+(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			+(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

			+(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			+(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

			-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			+(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			+(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		};
	}

	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}

	constexpr Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		const float w = (v.x * m._14) + (v.y * m._24) + (v.z * m._34) + (1.0f * m._44);
		return {
			((v.x * m._11) + (v.y * m._21) + (v.z * m._31) + (1.0f * m._41)) / w,
			((v.x * m._12) + (v.y * m._22) + (v.z * m._32) + (1.0f * m._42)) / w,
			((v.x * m._13) + (v.y * m._23) + (v.z * m._33) + (1.0f * m._43)) / w
		};
	}

	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		return {
			(v.x * m._11) + (v.y * m._21) + (v.z * m._31),
			(v.x * m._12) + (v.y * m._22) + (v.z * m._32),
			(v.x * m._13) + (v.y * m._23) + (v.z * m._33)
		};
	}

	constexpr Vector3 GetRight(const Matrix4& m)
	{
		return { m._11, m._12, m._13 };
	}

	constexpr Vector3 GetUp(const Matrix4& m)
	{
		return { m._21, m._22, m._23 };
	}

	constexpr Vector3 GetLook(const Matrix4& m)
	{
		return { m._31, m._32, m._33 };
	}

	constexpr Vector3 GetTranslation(const Matrix4& m)
	{
		return { m._41, m._42, m._43 };
	}

	inline float Magnitude(const Quaternion& q)
	{
		return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	}

	inline Quaternion Normalize(const Quaternion& q)
	{
		float length = Magnitude(q);
		return { q.w / length, q.x / length, q.y / length, q.z / length };
	}

	inline Quaternion Slerp(Quaternion& q0, Quaternion& q1, float t)
	{
		float dot = (q0.w * q1.w) + (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z);

		if (dot < 0.0f)
		{
			dot = -dot;
			q1 = -q1;
		}
		else if (dot > 0.999f)
		{
			return Normalize(Lerp(q0, q1, t));
		}

		float omega = acos(dot);
		float sinomega = sin(omega);

		float ratioA = sin((1.0f - t) * omega) / sinomega;
		float ratioB = sin(t * omega) / sinomega;

		return q0 * ratioA + q1 * ratioB;
	}

	bool Intersect(const Ray& ray, const Plane& plane, float& distance);
	bool Intersect(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal);

	bool Intersect(const Vector3& point, const AABB& aabb);
	bool Intersect(const Vector3& point, const OBB& obb);

	bool Intersect(const Vector3& point, const Plane& plane, float& penetration);
	bool Intersect(const Sphere& sphere, const Plane& plane, float& penetration);
}