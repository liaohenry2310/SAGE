#pragma once
#include "Service.h"

namespace Sage
{
	class AnimatorComponent;
	class ModelComponent;
	class ParticleEmitterComponent;
	class TransformComponent;

	class CameraService;
	class TerrainService;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

	private:
		friend class ModelComponent;
		friend class ParticleEmitterComponent;

		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

		void Register(ParticleEmitterComponent* component);
		void Unregister(ParticleEmitterComponent* component);

		struct Entry
		{
			const AnimatorComponent* animatorComponent = nullptr;
			const ModelComponent* modelComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		std::vector<Entry> mRenderEntries;
		std::vector<ParticleEmitterComponent*> mParticleEmitters;

		const CameraService* mCameraService = nullptr;
		const TerrainService* mTerrainService = nullptr;

		//TODO - turn these into components
		Graphics::DirectionalLight mDirectionalLight;

		Graphics::ShadowEffect mShadowEffect;
		Graphics::StandardEffect mStandardEffect;
		Graphics::TerrainEffect mTerrainEffect;
		Graphics::Skybox mSkyBox;

		float mFPS = 0.0f;
	};
}

