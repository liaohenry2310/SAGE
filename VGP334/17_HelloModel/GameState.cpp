#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

namespace
{
	const Math::Vector3 defaultLigthDir = Normalize({ 10.0f, -10.0f, 10.0f });
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 3.0f, -3.0f });

	mSkybox.Initialize("../../Assets/Images/space.jpg");

	mDirectionalLight.lightDirection = defaultLigthDir;
	mDirectionalLight.diffuse = Colors::White;
	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.specular = Colors::White;

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCmaera());
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseShadowMap() = true;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	//ModelIO::LoadModel("../../Assets/Models/Paladin/paladin.model", mCharacterModel);
	//ModelIO::LoadMaterial("../../Assets/Models/Paladin/paladin.material", mCharacterModel);
	//ModelIO::LoadSkeleton("../../Assets/Models/Paladin/paladin.skeleton", mCharacterModel);
	//ModelIO::LoadAnimationSet("../../Assets/Models/Paladin/paladin.animation", mCharacterModel);

	ModelIO::LoadModel("../../Assets/Models/Nightshade/nightshade.model", mCharacterModel);
	ModelIO::LoadMaterial("../../Assets/Models/Nightshade/nightshade.material", mCharacterModel);
	ModelIO::LoadSkeleton("../../Assets/Models/Nightshade/nightshade.skeleton", mCharacterModel);
	ModelIO::LoadAnimationSet("../../Assets/Models/Nightshade/nightshade.animation", mCharacterModel);

	mCharacterAnimator.Initialize(&mCharacterModel);
	mCharacterAnimator.PlayAnimation(0);

	mCharacter = CreateRenderGroup(mCharacterModel, &mCharacterAnimator);

	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(100, 100, 1, 10));
	mGround.diffuseMap.Initialize("../../Assets/Images/dirt_seamless.jpg");
	mGround.material.diffuse = Colors::Gray;
	mGround.material.ambient = Colors::Gray;
	mGround.material.specular = Colors::Black;

	mCamera.LookAt({0.0f, 0.0f, 0.0f});
}

void GameState::Terminate()
{
	mGround.Terminate();
	CleanUpRenderGroup(mCharacter);
	mSkybox.Terminate();
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float moveSpeed = 5.0f;
	const float rotateSpeed = 100.0f * Constants::DegToRad;
	const float turnSpeed = 30.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	mFPS = 1.0f / deltaTime;

	mCharacterAnimator.Update(deltaTime);
}

void GameState::Render()
{
	//Draw Depth
	{
		mShadowEffect.Begin();
		mShadowEffect.Render(mGround);
		mShadowEffect.RenderGroup(mCharacter);
		mShadowEffect.End();
	}

	//Draw scene
	{
		mSkybox.Render(mCamera);

		mStandardEffect.Begin();
		mStandardEffect.Render(mGround);
		

		if (mDrawSkeleton)
		{
			std::vector<Matrix4> boneTransforms;
			AnimationUtil::ComputeBoneTransforms(*mCharacterModel.skeleton, boneTransforms,
				[animator = &mCharacterAnimator](const Bone* bone)
			{
				return animator->GetTransform(bone);
			});
			AnimationUtil::DrawSkeleton(*mCharacterModel.skeleton, boneTransforms);
		}
		else
		{
			mStandardEffect.RenderGroup(mCharacter);
		}
		mStandardEffect.End();
	}

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	static Math::Vector3 tempLightDir = defaultLigthDir;
	static Math::Vector3 rotation = { 0.0f, 0.0f, 0.0f};

	ImGui::Begin("Debug Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader(" Lighting ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::SliderFloat3(" Light Direction (x, y, z) ", &tempLightDir.x, -10.0f, 10.0f))
		{
			mDirectionalLight.lightDirection = Math::Normalize(tempLightDir);
		}
		ImGui::ColorEdit4(" Light Ambient ", &mDirectionalLight.ambient.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Diffuse ", &mDirectionalLight.diffuse.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Specular ", &mDirectionalLight.specular.r, ImGuiColorEditFlags_NoAlpha);
		if (ImGui::ColorEdit4(" Material Ambient ", &mCharacter[0].material.ambient.r, ImGuiColorEditFlags_NoAlpha))
		{
			for (auto& object : mCharacter)
			{
				object.material = mCharacter[0].material;
			}
		}
		if (ImGui::ColorEdit4(" Material Diffuse ", &mCharacter[0].material.diffuse.r, ImGuiColorEditFlags_NoAlpha))
		{
			for (auto& object : mCharacter)
			{
				object.material = mCharacter[0].material;
			}
		}
		ImGui::ColorEdit4(" Material Specular ", &mCharacter[0].material.specular.r, ImGuiColorEditFlags_NoAlpha);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Shadow ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat(" Depth Bias ", &mStandardEffect.DepthBias(), 0.000001f, 0.0f, 1.0f, "%.6f");
	}

	ImGui::Checkbox(" Draw Skeleton ", &mDrawSkeleton);

	ImGui::End();
}
