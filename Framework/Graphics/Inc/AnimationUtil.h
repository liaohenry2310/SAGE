#pragma once

#include "Skeleton.h"

namespace Sage::Graphics::AnimationUtil
{
	using GetBoneTransform = std::function<Math::Matrix4(const Bone*)>;

	void ComputeBoneTransforms(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms, GetBoneTransform getBoneTransform);
	void ApplyBoneOffset(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms);

	void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneTransforms);
}