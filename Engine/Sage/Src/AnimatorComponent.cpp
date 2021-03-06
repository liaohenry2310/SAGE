#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"

using namespace Sage;
using namespace Sage::Graphics;

MEMORY_POOL_DEFINE(AnimatorComponent, 1000);

void AnimatorComponent::Initialize()
{
	mModelComponent = GetOwner().GetComponent<ModelComponent>();
	Model& model = mModelComponent->GetModel();

	for (auto& animationFileName : mAnimationFileNames)
		ModelIO::LoadAnimationSet(animationFileName, model);

	mAnimator.Initialize(&model);
}

void AnimatorComponent::Update(float deltaTime)
{
	mAnimator.Update(deltaTime);
}

void AnimatorComponent::DebugUI()
{

}

void AnimatorComponent::AddAnimation(std::string animationFileName)
{
	mAnimationFileNames.emplace_back(std::move(animationFileName));
}

void AnimatorComponent::PlayAnimation(size_t index)
{
	mAnimator.PlayAnimation(index);
}
