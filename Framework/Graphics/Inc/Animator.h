#pragma once

namespace Sage::Graphics
{
	struct Model;
	struct Bone;

	class Animator
	{
	public:
		void Initialize(const Model* model);

		void PlayAnimation(size_t clipIndex, bool looping = true);
		void Update(float deltaTime);

		float GetAnimationProgress() const;
		bool IsAnimationDone() const;

		Math::Matrix4 GetTransform(const Bone* bone) const;

	private:
		const Model* mModel = nullptr;
		size_t mClipIndex = 0;
		float mAnimationTick = 0.0f;
		bool mIsLooping = false;
	};
}