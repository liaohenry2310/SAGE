#include "Precompiled.h"
#include "EngineMath.h"

using namespace Sage::Math;

namespace
{
	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };
}

int Sage::Math::Random::UniformInt(int min, int max)
{
	return std::uniform_int_distribution<>{min, max}(myRandomEngine);
}

float Sage::Math::Random::UniformFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{min, max}(myRandomEngine);
}

float Sage::Math::Random::NormalFloat(float mean, float std)
{
	return std::normal_distribution<float>{mean, std}(myRandomEngine);
}

Vector2 Sage::Math::Random::InsideUnitCircle()
{
	return {
		UniformFloat(-1.0f, 1.0f),
		UniformFloat(-1.0f, 1.0f) 
	};
}

Vector2 Sage::Math::Random::OnUnitCircle()
{
	return Normalize({
		UniformFloat(-1.0f, 1.0f) + 0.001f,
		UniformFloat(-1.0f, 1.0f) 
	});
}

Vector3 Sage::Math::Random::InsideUnitSphere()
{
	return {
		UniformFloat(-1.0f, 1.0f),
		UniformFloat(-1.0f, 1.0f),
		UniformFloat(-1.0f, 1.0f)
	};
}

Vector3 Sage::Math::Random::OnUnitSphere()
{
	return Normalize({
		UniformFloat(-1.0f, 1.0f) + 0.001f,
		UniformFloat(-1.0f, 1.0f),
		UniformFloat(-1.0f, 1.0f)
	});
}