#include "Precompiled.h"
#include "StandardEffect.h"

#include "Animator.h"
#include "AnimationUtil.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Sage;
using namespace Sage::Math;
using namespace Sage::Graphics;

void StandardEffect::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
}

void StandardEffect::Terminate()
{
	mRasterizerState.Terminate();

	mSampler.Terminate();

	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mBoneTransformBuffer.BindVS(1);

	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindPS(3);

	mSettingsBuffer.BindVS(4);
	mSettingsBuffer.BindPS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void StandardEffect::End()
{
	if (mShadowMap)
		Texture::UnbindPS(4);
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "StandardEffect -- no camera set!");

	Matrix4 world = renderObject.transform.GetMatrix4();
	Matrix4 view = mCamera->GetViewMatrix();
	Matrix4 proj = mCamera->GetProjectionMatrix();

	TransformData transformData;
	transformData.world = Transpose(world);
	transformData.wvp[0] = Transpose(world * view * proj);
	if (mUseShadowMap && mLightCamera)
	{
		view = mLightCamera->GetViewMatrix();
		proj = mLightCamera->GetProjectionMatrix();
		transformData.wvp[1] = Transpose(world * view * proj);
	}
	transformData.viewPosition = mCamera->GetPosition();

	SettingsData settingsData;
	settingsData.useSpecularMap = mUseSpecularMap && renderObject.specularMap.GetRawData() ? 1 : 0;
	settingsData.useBumpMap = mUseDisplacementMap && renderObject.bumpMap.GetRawData() ? 1 : 0;
	settingsData.bumbWeight = Clamp(mDisplacementWeight, 0.0f, 1.0f);
	settingsData.useNormalMap = mUseNormalMap && renderObject.normalMap.GetRawData() ? 1 : 0;
	if (mUseShadowMap && mShadowMap)
	{
		mShadowMap->BindPS(4);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mDepthBias;
	}

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
	else if (renderObject.skeleton)
	{
		BoneTransformData boneTransformData;

		std::vector<Math::Matrix4> boneTransforms;
		AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, boneTransforms, [](const Bone* bone) {return bone->toParentTransform; });
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
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	renderObject.diffuseMap.BindPS(0);
	renderObject.specularMap.BindPS(1);
	renderObject.bumpMap.BindVS(2);
	renderObject.normalMap.BindPS(3);

	if (!renderObject.backfaceCulling)
		mRasterizerState.Set();

	renderObject.meshBuffer.Render();

	mRasterizerState.Clear();
}

void Sage::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::SetLightCamera(const Camera* camera)
{
	mLightCamera = camera;
}

void Sage::Graphics::StandardEffect::SetDirectionalLight(const DirectionalLight& light)
{
	mDirectionalLight = &light;
}

void Sage::Graphics::StandardEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}
