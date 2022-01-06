#pragma once

#include "BlendState.h"
#include "ConstantBuffer.h"
#include "DepthStencilState.h"
#include "Effect.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "Sampler.h"
#include "Texture.h"
#include "VertexShader.h"

namespace Sage::Graphics
{
	class Camera;

	class DissolveEffect : public Effect
	{
	public:
		DissolveEffect() = default;
		~DissolveEffect() = default;

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
		float& DissolveAmount() { return mDissolveAmount; }
		float& BurnAmount() { return mBurnAmount; }
		Color& Color1() { return mColor1; }
		Color& Color2() { return mColor2; }
		Color& Color3() { return mColor3; }

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
			int useSpecularMap = 0;
			int useBumpMap = 0;
			float bumbWeight = 0.0f;
			int useNormalMap = 0;

			int useShadowMap = 0;
			float depthBias = 0.0f;

			float dissolveAmount = 0.0f;
			float burnAmount = 0.0f;
			Color color1;
			Color color2;
			Color color3;
		};

		using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;
		using LightBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::DirectionalLight>;
		using MaterialBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::Material>;
		using SettingsBuffer = Sage::Graphics::TypedConstantBuffer<SettingsData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Texture mNoiseTexture;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		Sampler mSampler;
		RasterizerState mRasterizerState;
		BlendState mBlendState;

		bool mUseSpecularMap = false;
		bool mUseDisplacementMap = false;
		bool mUseNormalMap = false;
		bool mUseShadowMap = false;
		float mDisplacementWeight = 0.0f;
		float mDepthBias = 0.0001f;
		float mDissolveAmount = 0.0f;
		float mBurnAmount = 0.1f;
		Color mColor1 = Colors::Orange;
		Color mColor2 = Colors::OrangeRed;
		Color mColor3 = Colors::DarkRed;
	};
}