#pragma once

#include "ConstantBuffer.h"
#include "Effect.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace Sage::Graphics
{
	class Camera;
	class Terrain;

	class TerrainEffect : public Effect
	{
	public:
		TerrainEffect() = default;
		~TerrainEffect() = default;

		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;

		void Render(const RenderObject& renderObject) override;
		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera* camera);
		void SetDirectionalLight(const DirectionalLight& light);
		void SetShadowMap(const Texture* shadowMap);

		bool& UseShadowMap() { return mUseShadowMap; }
		float& DepthBias() { return mDepthBias; }

	private:
		struct TransformData
		{
			Sage::Math::Matrix4 world;
			Sage::Math::Matrix4 wvp[2];
			Sage::Math::Vector3 viewPosition;
			float padding;
		};

		struct SettingsData
		{
			int useShadowMap = 0;
			float depthBias = 0.0f;
			float padding[2] = {};
		};

		using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;
		using LightBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::DirectionalLight>;
		using MaterialBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::Material>;
		using SettingsBuffer = Sage::Graphics::TypedConstantBuffer<SettingsData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		Sampler mSampler;

		bool mUseShadowMap = false;
		float mDepthBias = 0.0001f;
	};
}
