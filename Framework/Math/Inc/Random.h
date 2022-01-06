#pragma once

namespace Sage::Math::Random
{
	int UniformInt(int min = 0, int max = std::numeric_limits<int>::max());

	float UniformFloat(float min = 0, float max = std::numeric_limits<float>::max());
	float NormalFloat(float mean, float std);

	Vector2 InsideUnitCircle();
	Vector2 OnUnitCircle();

	Vector3 InsideUnitSphere();
	Vector3 OnUnitSphere();
}