#include "Precompiled.h"
#include "Animator.h"

#include "Model.h"

using namespace Sage;
using namespace Sage::Graphics;

void Sage::Graphics::Animator::Initialize(const Model* model)
{
	mModel = model;
}

void Sage::Graphics::Animator::PlayAnimation(size_t clipIndex, bool looping)
{
	ASSERT(clipIndex < mModel->animationSet.size(), "Animator -- INvalid animation clip index.");

	if (mClipIndex != clipIndex)
	{
		mClipIndex = clipIndex;
		mAnimationTick = 0.0f;
		mIsLooping = looping;
	}
}

void Sage::Graphics::Animator::Update(float deltaTime)
{
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	mAnimationTick += clip.ticksPerSecond * deltaTime;
	if (mIsLooping && mAnimationTick > clip.tickDuration)
		mAnimationTick -= clip.tickDuration;
}

float Sage::Graphics::Animator::GetAnimationProgress() const
{
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	return Math::Clamp(mAnimationTick / clip.tickDuration, 0.0f, 1.0f);
}

bool Sage::Graphics::Animator::IsAnimationDone() const
{
	return !mIsLooping && (mAnimationTick > mModel->animationSet[mClipIndex].tickDuration);
}

Math::Matrix4 Sage::Graphics::Animator::GetTransform(const Bone* bone) const
{
	Math::Matrix4 toParentTransform = bone->toParentTransform;
	
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	if (clip.HasTransform(bone->index))
		toParentTransform = clip.boneAnimations[bone->index]->GetTransform(mAnimationTick);

	return toParentTransform;
}
