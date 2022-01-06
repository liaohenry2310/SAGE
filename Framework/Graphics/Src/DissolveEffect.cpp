#include "Precompiled.h"
#include "DissolveEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Sage;
using namespace Sage::Math;
using namespace Sage::Graphics;

void DissolveEffect::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/Dissolve.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Dissolve.fx");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mNoiseTexture.Initialize("../../Assets/Images/perlin_noise.jpg");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void DissolveEffect::Terminate()
{
	mBlendState.Terminate();
	mRasterizerState.Terminate();
	mSampler.Terminate();

	mNoiseTexture.Terminate();

	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void DissolveEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mRasterizerState.Set();
	mBlendState.Set();
}

void DissolveEffect::End()
{
	TransformBuffer::UnBindVS(0);
	LightBuffer::UnBindVS(1);
	LightBuffer::UnBindPS(1);
	MaterialBuffer::UnBindPS(2);
	SettingsBuffer::UnBindVS(3);
	SettingsBuffer::UnBindPS(3);

	if (mShadowMap)
		Texture::UnbindPS(4);

	mRasterizerState.Clear();
	mBlendState.ClearState();
}

void DissolveEffect::Render(const RenderObject& renderObject)
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
	settingsData.dissolveAmount = Clamp(mDissolveAmount, 0.0f, 1.0f);
	settingsData.burnAmount = Clamp(mBurnAmount, 0.0f, mDissolveAmount);
	settingsData.color1 = mColor1;
	settingsData.color2 = mColor2;
	settingsData.color3 = mColor3;
	if (mUseShadowMap && mShadowMap)
	{
		mShadowMap->BindPS(4);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mDepthBias;
	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	renderObject.diffuseMap.BindPS(0);
	renderObject.specularMap.BindPS(1);
	renderObject.bumpMap.BindVS(2);
	renderObject.normalMap.BindPS(3);
	mNoiseTexture.BindPS(5);

	renderObject.meshBuffer.Render();
}

void Sage::Graphics::DissolveEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void DissolveEffect::SetLightCamera(const Camera* camera)
{
	mLightCamera = camera;
}

void Sage::Graphics::DissolveEffect::SetDirectionalLight(const DirectionalLight& light)
{
	mDirectionalLight = &light;
}

void Sage::Graphics::DissolveEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}
