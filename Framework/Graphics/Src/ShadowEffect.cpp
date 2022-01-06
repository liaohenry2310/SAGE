#include "Precompiled.h"
#include "ShadowEffect.h"

#include "Animator.h"
#include "AnimationUtil.h"
#include "VertexTypes.h"

void Sage::Graphics::ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);

	mVertexShader.Initialize("../../Assets/Shaders/Shadow.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Shadow.fx");

	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();

	constexpr uint32_t depthMapSize = 4096;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_U32);
}

void Sage::Graphics::ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();

	mSettingsBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void Sage::Graphics::ShadowEffect::Begin()
{
	UpdateLightCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mBoneTransformBuffer.BindVS(1);
	mSettingsBuffer.BindVS(2);

	mDepthMapRenderTarget.BeginRender();
}

void Sage::Graphics::ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void Sage::Graphics::ShadowEffect::Render(const RenderObject& renderObject)
{
	Math::Matrix4 world = renderObject.transform.GetMatrix4();
	Math::Matrix4 view = mLightCamera.GetViewMatrix();
	Math::Matrix4 proj = mLightCamera.GetProjectionMatrix();

	TransformData transformData;
	transformData.wvp = Math::Transpose(world * view * proj);

	SettingsData settingsData;

	if (renderObject.animator)
	{
		BoneTransformData boneTransformData;

		std::vector<Math::Matrix4> boneTransforms;
		AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, boneTransforms,
			[animator = renderObject.animator](const Bone* bone){ return animator->GetTransform(bone); });
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		const size_t boneCount = renderObject.skeleton->bones.size();
		for (size_t i = 0; i < boneCount && i < BoneTransformData::MaxBoneCount; ++i)
		{
			boneTransformData.boneTransforms[i] = Math::Transpose(boneTransforms[i]);
		}

		mBoneTransformBuffer.Update(boneTransformData);
		settingsData.useSkinning = 1;
	}

	mTransformBuffer.Update(transformData);
	mSettingsBuffer.Update(settingsData);
	renderObject.meshBuffer.Render();
}

void Sage::Graphics::ShadowEffect::SetDirectionalLight(const DirectionalLight& light)
{
	mDirectionalLight = &light;
}

void Sage::Graphics::ShadowEffect::SetCameraFollow(const Camera& cameraToFollow)
{
	mCameraFollow = &cameraToFollow;
}

void Sage::Graphics::ShadowEffect::UpdateLightCamera()
{
	ASSERT(mDirectionalLight != nullptr, "ShadowEffect -- no light set!");

	const Math::Vector3 direction = Math::Normalize(mDirectionalLight->lightDirection);
	mLightCamera.SetDirection(direction);
	const Math::Vector3 offset = -(direction * 100.0f);
	mLightCamera.SetPosition(mCameraFollow ? mCameraFollow->GetPosition() + offset : offset);
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(1500.0f);
	mLightCamera.SetSize(50.0f, 50.0f);
}
