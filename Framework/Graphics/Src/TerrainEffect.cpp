#include "Precompiled.h"
#include "TerrainEffect.h"

#include "Animator.h"
#include "AnimationUtil.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Sage;
using namespace Sage::Math;
using namespace Sage::Graphics;

void TerrainEffect::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/Terrain.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Terrain.fx");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
	mSampler.Terminate();

	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void TerrainEffect::Begin()
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
}

void TerrainEffect::End()
{
	if (mShadowMap != nullptr)
		Texture::UnbindPS(1);
}

void TerrainEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "Terrain Effect -- no camera set!");

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
	if (mUseShadowMap && mShadowMap)
	{
		mShadowMap->BindPS(1);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mDepthBias;
	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	renderObject.diffuseMap.BindPS(0);
	renderObject.meshBuffer.Render();
}

void TerrainEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void TerrainEffect::SetLightCamera(const Camera* camera)
{
	mLightCamera = camera;
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight& light)
{
	mDirectionalLight = &light;
}

void TerrainEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}
