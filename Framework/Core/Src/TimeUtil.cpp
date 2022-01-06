#include "Precompiled.h"
#include "TimeUtil.h"

float Sage::Core::TimeUtil::GetTime()
{
	static const auto startTime = std::chrono::high_resolution_clock::now();
	const auto now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0f;
}

float Sage::Core::TimeUtil::GetDeltaTime()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;
	return deltaTime;
}
