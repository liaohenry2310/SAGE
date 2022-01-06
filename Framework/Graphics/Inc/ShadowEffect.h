#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "Effect.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "VertexShader.h"

namespace Sage::Graphics
{
	class ShadowEffect final : public Effect
	{
	public:
		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;

		void Render(const RenderObject& renderObject) override;

		void SetDirectionalLight(const DirectionalLight& light);
		void SetCameraFollow(const Camera& cameraToFollow);

		const Camera& GetLightCmaera() const { return mLightCamera; }
		const Texture& GetDepthMap() const { return mDepthMapRenderTarget; }

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		struct BoneTransformData
		{
			static constexpr int MaxBoneCount = 128;
			Math::Matrix4 boneTransforms[MaxBoneCount];
		};

		struct SettingsData
		{
			int useSkinning = 0;
			float padding[3];
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;
		using SettingsBuffer = Sage::Graphics::TypedConstantBuffer<SettingsData>;

		const DirectionalLight* mDirectionalLight = nullptr;
		const Camera* mCameraFollow = nullptr;

		Camera mLightCamera;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		SettingsBuffer mSettingsBuffer;
		RenderTarget mDepthMapRenderTarget;

		void UpdateLightCamera();
	};
}