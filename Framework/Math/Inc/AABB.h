#pragma once

namespace Sage::Math
{
	struct AABB
	{
		Vector3 center = Vector3::Zero;
		Vector3 extend = Vector3::One;

		constexpr Vector3 Min() const { return Vector3{}; }
		constexpr Vector3 Max() const { return Vector3{}; }
	};
}