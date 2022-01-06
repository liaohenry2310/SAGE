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

	class StandardEffect : public Effect
	{
	public:
		StandardEffect() = default;
		~StandardEffect() = default;

		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;

		void Render(const RenderObject& renderObject) override;
		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera* camera);
		void SetDirectionalLight(const DirectionalLight& light);
		void SetShadowMap(const Texture* shadowMap);

		bool& UseSpecularMap() { return mUseSpecularMap; }
		bool& UseDisplacementMap() { return mUseDisplacementMap; }
		bool& UseNormalMap() { return mUseNormalMap; }
		bool& UseShadowMap() { return mUseShadowMap; }
		float& DisplacementWeight() { return mDisplacementWeight; }
		float& DepthBias() { return mDepthBias; }

	private:
		struct TransformData
		{
			Sage::Math::Matrix4 world;
			Sage::Math::Matrix4 wvp[2];
			Sage::Math::Vector3 viewPosition;
			float padding;
		};

		struct BoneTransformData
		{
			static constexpr int MaxBoneCount = 128;
			Math::Matrix4 boneTransforms[MaxBoneCount];
		};

		struct SettingsData
		{
			int useSpecularMap = 0;
			int useBumpMap = 0;
			float bumbWeight = 0.0f;
			int useNormalMap = 0;

			int useShadowMap = 0;
			int useSkinning = 0;
			float depthBias = 0.0f;
			float padding[1] = {};
		};

		using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = Sage::Graphics::TypedConstantBuffer<BoneTransformData>;
		using LightBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::DirectionalLight>;
		using MaterialBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::Material>;
		using SettingsBuffer = Sage::Graphics::TypedConstantBuffer<SettingsData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		RasterizerState mRasterizerState;

		bool mUseSpecularMap = false;
		bool mUseDisplacementMap = false;
		bool mUseNormalMap = false;
		bool mUseShadowMap = false;
		float mDisplacementWeight = 0.0f;
		float mDepthBias = 0.00001f;
	};
}