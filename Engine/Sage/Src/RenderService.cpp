#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "ModelComponent.h"
#include "ParticleEmitterComponent.h"
#include "TerrainService.h"
#include "TransformComponent.h"

using namespace Sage;
using namespace Sage::Graphics;

void Sage::RenderService::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::White);

	mCameraService = GetWorld().GetService<CameraService>();
	mTerrainService = GetWorld().GetService<TerrainService>();

	mSkyBox.Initialize("../../Assets/Images/skybox.png");

	mDirectionalLight.lightDirection = Math::Normalize({ 0.75f, -1.0f, 0.75f });
	mDirectionalLight.diffuse = Colors::White;
	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.specular = Colors::White;

	mStandardEffect.Initialize();
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCmaera());
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseSpecularMap() = true;
	mStandardEffect.UseDisplacementMap() = false;
	mStandardEffect.UseNormalMap() = true;
	mStandardEffect.UseShadowMap() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;
	mStandardEffect.DepthBias() = 0.001f;

	mTerrainEffect.Initialize();
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetLightCamera(&mShadowEffect.GetLightCmaera());
	mTerrainEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mTerrainEffect.UseShadowMap() = true;
	mTerrainEffect.DepthBias() = 0.0001f;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void Sage::RenderService::Terminate()
{
	mShadowEffect.Terminate();
	mTerrainEffect.Terminate();
	mStandardEffect.Terminate();
	mSkyBox.Terminate();
}

void Sage::RenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void Sage::RenderService::Render()
{
	mSkyBox.Render(mCameraService->GetCamera());

	mStandardEffect.SetCamera(mCameraService->GetCamera());
	mTerrainEffect.SetCamera(mCameraService->GetCamera());
	mShadowEffect.SetCameraFollow(mCameraService->GetCamera());

	for (auto& entry : mRenderEntries)
	{
		for (auto& renderObject : entry.renderGroup)
		{
			auto angles = entry.modelComponent->GetRotation();
			auto rotation = Math::Quaternion::RotationEuler(angles);
			auto transform = *(static_cast<const Graphics::Transform*>(entry.transformComponent));
			transform.rotation = rotation * transform.rotation;
			renderObject.transform = transform;
		}
	}

	mShadowEffect.Begin();
	for (auto& entry : mRenderEntries)
		mShadowEffect.RenderGroup(entry.renderGroup);
	if (mTerrainService)
		mShadowEffect.Render(mTerrainService->GetTerrainRenderObject());
	mShadowEffect.End();

	mStandardEffect.Begin();
	for (auto& entry : mRenderEntries)
		mStandardEffect.RenderGroup(entry.renderGroup);
	mStandardEffect.End();

	if (mTerrainService)
	{
		mTerrainEffect.Begin();
		mTerrainEffect.Render(mTerrainService->GetTerrainRenderObject());
		mTerrainEffect.End();
	}

	for (auto p : mParticleEmitters)
	{
		p->GetParticleEmitter().Render(mCameraService->GetCamera());
	}

	SimpleDraw::Render(mCameraService->GetCamera());
}

void Sage::RenderService::DebugUI()
{
	SimpleDraw::AddGroundPlane(10);
	SimpleDraw::AddTransform(Math::Matrix4::Identity);

	for (auto& entry : mRenderEntries)
		entry.transformComponent->GetOwner().DebugUI();

	SimpleDraw::Render(mCameraService->GetCamera());

	ImGui::Begin("RenderService Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	ImGui::SliderFloat3("Light Direction", &mDirectionalLight.lightDirection.x, -10.0f, 10.0f);
	ImGui::End();
}

void Sage::RenderService::Register(const ModelComponent* modelComponent)
{
	Entry& entry = mRenderEntries.emplace_back();
	auto& gameObject = modelComponent->GetOwner();

	entry.animatorComponent = gameObject.GetComponent<AnimatorComponent>();
	entry.modelComponent = modelComponent;
	entry.transformComponent = gameObject.GetComponent<TransformComponent>();

	const Animator* animator = nullptr;
	if (entry.animatorComponent)
		animator = &entry.animatorComponent->GetAnimator();
	entry.renderGroup = CreateRenderGroup(modelComponent->GetModel(), animator);
}

void Sage::RenderService::Unregister(const ModelComponent* modelComponent)
{
	auto match = [&](const auto& entry) { return entry.modelComponent == modelComponent; };
	auto iter = std::find_if(mRenderEntries.begin(), mRenderEntries.end(), match);
	if (iter != mRenderEntries.end())
	{
		Entry& entry = *iter;
		CleanUpRenderGroup(entry.renderGroup);
		mRenderEntries.erase(iter);
	}
}

void Sage::RenderService::Register(ParticleEmitterComponent* component)
{
	mParticleEmitters.emplace_back(component);
}

void Sage::RenderService::Unregister(ParticleEmitterComponent* component)
{
	mParticleEmitters.erase(std::remove_if(mParticleEmitters.begin(), mParticleEmitters.end(), [&](auto p) { return p == component; }));
}
