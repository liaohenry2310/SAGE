#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

namespace
{
	const Math::Vector3 defaultLigthDir = Normalize({ 10.0f, -10.0f, 0.0f });
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

	mDissolveEffect.Initialize();
	mDissolveEffect.SetCamera(mCamera);
	mDissolveEffect.SetDirectionalLight(mDirectionalLight);
	mDissolveEffect.SetLightCamera(&mShadowEffect.GetLightCmaera());
	mDissolveEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mDissolveEffect.UseShadowMap() = true;
	mDissolveEffect.UseNormalMap() = true;
	mDissolveEffect.UseSpecularMap() = true;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	Model model;
	ModelIO::LoadModel("../../Assets/Models/SpaceShip/spaceship.model", model);

	mObject.meshBuffer.Initialize(model.meshData[0].mesh);
	mObject.diffuseMap.Initialize("../../Assets/Models/SpaceShip/spaceship_diffuse.jpg");
	mObject.specularMap.Initialize("../../Assets/Models/SpaceShip/spaceship_specular.jpg");
	mObject.normalMap.Initialize("../../Assets/Models/SpaceShip/spaceship_normal.jpg");
	mObject.transform.position = { 0.0f, 1.0f, 0.0f };
	mObject.transform.rotation = Quaternion::RotationEuler({ Constants::Pi * 0.5f, 0.0f, 0.0f });
	mObject.transform.scale = { 0.2f, 0.2f, 0.2f };
	mObject.material.diffuse = Colors::White;
	mObject.material.ambient = Colors::White;
	mObject.material.specular = Colors::White;
	mObject.material.power = 5.0f;

	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(100, 100, 1, 10));
	mGround.diffuseMap.Initialize("../../Assets/Images/dirt_seamless.jpg");
	mGround.material.diffuse = Colors::Gray;
	mGround.material.ambient = Colors::Gray;
	mGround.material.specular = Colors::Black;

	mCamera.LookAt(mObject.transform.position);

	mAnimation = AnimationBuilder()
		.AddPositionKey({ 5.0f, 3.0f, 5.0f }, 0.0f)
		.AddPositionKey({ 5.0f, 3.0f, -5.0f }, 1.0f)
		.AddPositionKey({ -5.0f, 3.0f, -5.0f }, 2.0f)
		.AddPositionKey({ -5.0f, 3.0f, 5.0f }, 3.0f)
		.AddPositionKey({ 5.0f, 3.0f, 5.0f }, 4.0f)
		.AddRotationKey(Quaternion::RotationEuler({ Constants::Pi * 0.5f, 0.0f, -0.5f }), 0.0f)
		.AddRotationKey(Quaternion::RotationEuler({ Constants::Pi * 0.5f, Constants::Pi * 0.5f, 0.5f }), 1.0f)
		.AddRotationKey(Quaternion::RotationEuler({ Constants::Pi * 0.5f, Constants::Pi * 1.0f, -0.5f }), 2.0f)
		.AddRotationKey(Quaternion::RotationEuler({ Constants::Pi * 0.5f, Constants::Pi * 1.5f, 0.5f }), 3.0f)
		.AddRotationKey(Quaternion::RotationEuler({ Constants::Pi * 0.5f, Constants::Pi * 2.0f, -0.5f }), 4.0f)
		.AddScaleKey({ 0.2f, 0.2f, 0.2f }, 0.0f)
		.Get();
}

void GameState::Terminate()
{
	mGround.Terminate();
	mObject.Terminate();
	mSkybox.Terminate();
	mShadowEffect.Terminate();
	mDissolveEffect.Terminate();
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

	if (mPlayDissolve)
		mDissolveEffect.DissolveAmount() = Clamp(mDissolveEffect.DissolveAmount() + (1.0f / mDissolveTime) * deltaTime, 0.0f, 1.0f);
	if (mDissolveEffect.DissolveAmount() >= 1.0f)
		mPlayDissolve = false;

	mTime += deltaTime;
	if (mTime > mAnimation.GetDuration())
		mTime = 0.0f;
}

void GameState::Render()
{
	//Draw Depth
	{
		mShadowEffect.Begin();
		mShadowEffect.Render(mGround);
		mShadowEffect.Render(mObject);
		mShadowEffect.End();
	}

	//Draw scene
	{
		mSkybox.Render(mCamera);

		mStandardEffect.Begin();
		mStandardEffect.Render(mGround);
		mStandardEffect.End();

		mDissolveEffect.Begin();
		mDissolveEffect.Render(mObject);
		mDissolveEffect.End();
	}
}

void GameState::DebugUI()
{
	static Math::Vector3 tempLightDir = defaultLigthDir;
	static Math::Vector3 rotation = { Constants::Pi * 0.5f, 0.0f, 0.0f};

	ImGui::Begin("Debug Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader(" SpaceShip ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button(" Reset Rotation "))
		{
			rotation = { Constants::Pi * 0.5f, 0.0f, 0.0f };
			mObject.transform.rotation = Quaternion::RotationEuler({ Constants::Pi * 0.5f, 0.0f, 0.0f });
		}

		if (ImGui::SliderFloat3(" Rotation ", &rotation.x, -Constants::TwoPi, Constants::TwoPi))
		{
			mObject.transform.rotation = Quaternion::RotationEuler(rotation);
		}
	}

	if (ImGui::CollapsingHeader(" Lighting ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::SliderFloat3(" Light Direction (x, y, z) ", &tempLightDir.x, -10.0f, 10.0f))
		{
			mDirectionalLight.lightDirection = Math::Normalize(tempLightDir);
		}
		ImGui::ColorEdit4(" Light Ambient ", &mDirectionalLight.ambient.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Diffuse ", &mDirectionalLight.diffuse.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Specular ", &mDirectionalLight.specular.r, ImGuiColorEditFlags_NoAlpha);
	}

	if (ImGui::CollapsingHeader(" Dissolve Effect ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button(" Play ") && !mPlayDissolve)
		{
			mPlayDissolve = true;
			mDissolveEffect.DissolveAmount() = 0.0f;
		}
		if (ImGui::Button(" Reset Dissolve "))
		{
			mPlayDissolve = false;
			mDissolveEffect.DissolveAmount() = 0.0f;
		}
		ImGui::SliderFloat(" Dissolve Time ", &mDissolveTime, 0.1f, 5.0f);
		ImGui::SliderFloat(" Dissolve Amount ", &mDissolveEffect.DissolveAmount(), 0.0f, 1.0f);
		ImGui::SliderFloat(" Burn Amount ", &mDissolveEffect.BurnAmount(), 0.0f, mDissolveEffect.DissolveAmount());
		ImGui::ColorEdit4(" Burn Color 1 ", &mDissolveEffect.Color1().r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Burn Color 2 ", &mDissolveEffect.Color2().r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Burn Color 3 ", &mDissolveEffect.Color3().r, ImGuiColorEditFlags_NoAlpha);
		static bool shadow = false;
		if (ImGui::Checkbox(" Shadow ", &shadow))
		{
			mStandardEffect.UseShadowMap() = shadow;
			mDissolveEffect.UseShadowMap() = shadow;
		}
	}

	ImGui::End();
}
