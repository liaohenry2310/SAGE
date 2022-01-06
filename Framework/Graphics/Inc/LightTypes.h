#pragma once
#include "Colors.h"

namespace Sage::Graphics
{
	struct DirectionalLight
	{
		Sage::Math::Vector3 lightDirection;
		float padding = 0.0f;
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
	};
}