#pragma once

#include "Animation.h"

namespace Sage::Graphics
{
	class AnimationClip
	{
	public:
		bool HasTransform(uint32_t boneIndex) const
		{
			const auto& boneAnime = boneAnimations[boneIndex];
			return (boneAnime != nullptr);
		}

		std::string name;
		float tickDuration = 0.0f;
		float ticksPerSecond = 0.0f;
		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}