#pragma once

#include "Animation.h"

namespace Sage::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(const Math::Vector3& position, float time);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation, float time);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time);

		[[nodiscard]] Animation Get();

	private:
		Animation mWorkingCopy;
	};
}