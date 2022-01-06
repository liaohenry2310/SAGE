#pragma once

#include "Colors.h"

namespace Sage::Graphics
{
	struct Material
	{
		Color ambient = Colors::White;
		Color diffuse = Colors::White;	
		Color specular = Colors::White;
		float power = 1.0f;
		float padding[3] = { 0.0f };
	};
}