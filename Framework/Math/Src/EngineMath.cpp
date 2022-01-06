#include "Precompiled.h"
#include "EngineMath.h"

using namespace Sage::Math;

const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f, 1.0f };
const Vector2 Vector2::XAxis{ 1.0f, 0.0f };
const Vector2 Vector2::YAxis{ 0.0f, 1.0f };

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f, 1.0f, 1.0f };
const Vector3 Vector3::XAxis{ 1.0f, 0.0f, 0.0f };
const Vector3 Vector3::YAxis{ 0.0f, 1.0f, 0.0f };
const Vector3 Vector3::ZAxis{ 0.0f, 0.0f, 1.0f };

const Matrix4 Matrix4::Identity
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

const Quaternion Quaternion::Identity { 1.0f, 0.0f, 0.0f, 0.0f };

Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return {
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return {
		1 - (2 * q.y * q.y) - (2 * q.z * q.z),
		(2 * q.x * q.y) + (2 * q.w * q.z),
		(2 * q.x * q.z) - (2 * q.w * q.y),
		0,

		(2 * q.x * q.y) - (2 * q.w * q.z),
		1 - (2 * q.x * q.x) - (2 * q.z * q.z),
		(2 * q.y * q.z) + (2 * q.w * q.x),
		0,

		(2 * q.x * q.z) + (2 * q.w * q.y),
		(2 * q.y * q.z) - (2 * q.w * q.x),
		1 - (2 * q.x * q.x) - (2 * q.y * q.y),
		0,

		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad)
{
	Vector3 normalized = Normalize(axis);

	return { 
		cos(rad * 0.5f),
		normalized.x * sin(rad * 0.5f),
		normalized.y * sin(rad * 0.5f),
		normalized.z * sin(rad * 0.5f)
	};
}

Quaternion Quaternion::RotationEuler(const Vector3& eulerAngles)
{
	const float cr = cos(eulerAngles.x * 0.5f);
	const float sr = sin(eulerAngles.x * 0.5f);
	const float cy = cos(eulerAngles.z * 0.5f);
	const float sy = sin(eulerAngles.z * 0.5f);
	const float cp = cos(eulerAngles.y * 0.5f);
	const float sp = sin(eulerAngles.y * 0.5f);

	return {
		cr * cp * cy + sr * sp * sy,
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy
	};

}

Quaternion Quaternion::RotationMatrix(const Matrix4& _m)
{
	Matrix4 m = Transpose(_m);

	Quaternion q;
	float trace = m._11 + m._22 + m._33; 

	if (trace > 0) 
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (m._32 - m._23) * s;
		q.y = (m._13 - m._31) * s;
		q.z = (m._21 - m._12) * s;
	}
	else 
	{
		if (m._11 > m._22 && m._11 > m._33)
		{
			float s = 2.0f * sqrtf(1.0f + m._11 - m._22 - m._33);
			q.w = (m._32 - m._23) / s;
			q.x = 0.25f * s;
			q.y = (m._12 + m._21) / s;
			q.z = (m._13 + m._31) / s;
		}
		else if (m._22 > m._33) 
		{
			float s = 2.0f * sqrtf(1.0f + m._22 - m._11 - m._33);
			q.w = (m._13 - m._31) / s;
			q.x = (m._12 + m._21) / s;
			q.y = 0.25f * s;
			q.z = (m._23 + m._32) / s;
		}
		else 
		{
			float s = 2.0f * sqrtf(1.0f + m._33 - m._11 - m._22);
			q.w = (m._21 - m._12) / s;
			q.x = (m._13 + m._31) / s;
			q.y = (m._23 + m._32) / s;
			q.z = 0.25f * s;
		}
	}

	return q;
}

Quaternion Quaternion::RotationLook(const Vector3& direction, const Vector3& up)
{
	const Vector3 l = Normalize(direction);
	const Vector3 r = Normalize(Cross(up, l));
	const Vector3 u = Normalize(Cross(l, r));

	Matrix4 m{
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		l.x, l.y, l.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return RotationMatrix(m);
}

Quaternion Quaternion::RotationFromTo(const Vector3& from, const Vector3& to)
{
	Vector3 f = Normalize(from);
	Vector3 t = Normalize(to);

	Vector3 axis = Cross(f, t);
	float angle = acos(Dot(t, f));

	return Quaternion::RotationAxis(axis, angle);
}

bool Sage::Math::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float originDotN = Dot(ray.origin, plane.normal);
	const float directionDotN = Dot(ray.direction, plane.normal);

	if (directionDotN == 0.0f)
	{
		if (originDotN == plane.distance)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	distance = (plane.distance - originDotN) / directionDotN;
	return true;
}

bool Sage::Math::Intersect(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	// Compute the local-to-world/world-to-local matrices
	const Matrix4 matTrans = Matrix4::Translation(obb.center);
	const Matrix4 matRot = Matrix4::RotationQuaternion(obb.rotation);
	const Matrix4 matWorld = matRot * matTrans;
	const Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	const Vector3 org = TransformCoord(ray.origin, matWorldInv);
	const Vector3 dir = TransformNormal(ray.direction, matWorldInv);
	const Ray localRay{ org, dir };

	Plane planes[] =
	{
		{ {  0.0f,  0.0f, -1.0f }, obb.extend.z },
		{ {  0.0f,  0.0f,  1.0f }, obb.extend.z },
		{ {  0.0f, -1.0f,  0.0f }, obb.extend.y },
		{ {  0.0f,  1.0f,  0.0f }, obb.extend.y },
		{ { -1.0f,  0.0f,  0.0f }, obb.extend.x },
		{ {  1.0f,  0.0f,  0.0f }, obb.extend.x }
	};

	uint32_t numIntersections = 0;
	for (auto& plane : planes)
	{
		const float d = Dot(org, plane.normal);
		if (d > plane.distance)
		{
			float distance = 0.0f;
			if (Intersect(localRay, plane, distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = plane.normal;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}

bool Sage::Math::Intersect(const Vector3& point, const AABB& aabb)
{
	const auto min = aabb.Min();
	const auto max = aabb.Max();

	if (point.x < min.x || point.x > max.x ||
		point.y < min.y || point.y > max.y ||
		point.z < min.z || point.z > max.z)
		return false;

	return true;
}

bool Sage::Math::Intersect(const Vector3& point, const OBB& obb)
{
	const Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	const Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rotation);
	const Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	const Math::Matrix4 transform = matScale * matRot * matTrans;

	const Vector3 localPoint = TransformCoord(point, Inverse(transform));
	const AABB unitAABB = { Vector3::Zero, Vector3::One };

	return Intersect(localPoint, unitAABB);
}

bool Sage::Math::Intersect(const Vector3& point, const Plane& plane, float& penetration)
{
	const float centerDistance = Dot(point, plane.normal);
	const float distToPlane = centerDistance - plane.distance;

	if (distToPlane > 0.0f)
		return false;

	penetration = plane.distance - centerDistance;
	return true;
}

bool Sage::Math::Intersect(const Sphere& sphere, const Plane& plane, float& penetration)
{
	const float centerDistance = Dot(sphere.center, plane.normal);
	const float distToPlane = centerDistance - plane.distance - sphere.radius;

	if (distToPlane > 0.0f)
		return false;

	penetration = plane.distance - centerDistance + sphere.radius;
	return true;
}
