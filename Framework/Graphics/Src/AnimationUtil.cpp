#include "Precompiled.h"
#include "AnimationUtil.h"

#include "SimpleDraw.h"

using namespace Sage;
using namespace Sage::Graphics;

namespace
{
	void ComputeTransform(Bone* bone, std::vector<Math::Matrix4>& boneTransforms, AnimationUtil::GetBoneTransform getBoneTransform)
	{
		if (bone->parent)
			boneTransforms[bone->index] = getBoneTransform(bone) * boneTransforms[bone->parentIndex];

		for (auto child : bone->children)
			ComputeTransform(child, boneTransforms, getBoneTransform);
	}
}

void Sage::Graphics::AnimationUtil::ComputeBoneTransforms(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms, GetBoneTransform getBoneTransform)
{
	boneTransforms.resize(skeleton.bones.size(), Math::Matrix4::Identity);
	ComputeTransform(skeleton.root, boneTransforms, getBoneTransform);
}

void Sage::Graphics::AnimationUtil::ApplyBoneOffset(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms)
{
	for (auto& bone : skeleton.bones)
	{
		boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
	}
}

void AnimationUtil::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneTransforms)
{
	for (auto& bone : skeleton.bones)
	{
		const Bone* parentBone = bone->parent;
		if (parentBone)
		{
			const Math::Vector3& bonePosition = Math::GetTranslation(boneTransforms[bone->index]);
			const Math::Vector3& parentPosition = Math::GetTranslation(boneTransforms[parentBone->index]);
			SimpleDraw::AddSphere(bonePosition, 0.02f, Colors::Aqua, 5, 5);
			SimpleDraw::AddCone(bonePosition, parentPosition - bonePosition, 0.02f, Math::Distance(parentPosition, bonePosition), Colors::Aqua, 0, 5);
		}
	}
}
