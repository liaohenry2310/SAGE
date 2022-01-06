#pragma once
#include "Component.h"

namespace Sage
{
	class ParticleEmitterComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::ParticleEmitter);
		MEMORY_POOL_DECLARE;

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void DebugUI() override;

		void SetPosOffset(Math::Vector3 offset) { mPosOffset = offset; }

		const Graphics::ParticleEmitter& GetParticleEmitter() const { return mParticleEmitter; }
		Graphics::ParticleEmitter& GetParticleEmitter() { return mParticleEmitter; }

	private:
		Graphics::ParticleEmitter mParticleEmitter;
		Math::Vector3 mPosOffset = Math::Vector3::Zero;
	};
}