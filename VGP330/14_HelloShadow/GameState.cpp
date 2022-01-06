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

	mCamera.SetPosition({ 0.0f, 5.0f, -5.0f });

	mSkybox.Initialize("../../Assets/Images/space.jpg");

	mDirectionalLight.lightDirection = defaultLigthDir;
	mDirectionalLight.diffuse = Colors::White;
	mDirectionalLight.ambient = Colors::Gray;
	mDirectionalLight.specular = Colors::Gray;

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCmaera());
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseShadowMap() = true;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	mObject.meshBuffer.Initialize(MeshBuilder::CreatSphere(32, 32));
	mObject.diffuseMap.Initialize("../../Assets/Images/moon.jpg");
	mObject.transform.position = { 0.0f, 1.0f, 0.0f };
	mObject.transform.scale = { 2.0f, 2.0f, 2.0f };
	mObject.material.specular = Colors::Gray;
	mObject.material.power = 5.0f;

	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(100, 100, 1, 10));
	mGround.diffuseMap.Initialize("../../Assets/Images/dirt_seamless.jpg");
	mGround.material.specular = Colors::Black;

	mCamera.LookAt(mObject.transform.position);
}

void GameState::Terminate()
{
	mGround.Terminate();
	mObject.Terminate();
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
	mTime += deltaTime;
	mObject.transform.position.y = 2.0f + sinf(mTime) * 0.5f;
}

void GameState::Render()
{
	const int objCount = 10;
	std::vector<Vector3> offsets(objCount);
	for (int i = 0; i < objCount; ++i)
	{
		float distance = 2.0f * i;
		float height = 1.0f + 1.0f * i;
		float angle = 1.0f * i;
		offsets[i] = { cos(angle) * distance, height, sin(angle) * distance };
	}


	//Draw depth map
	{
		mShadowEffect.Begin();
		for (int i = 0; i < objCount; ++i)
		{
			mObject.transform.position = offsets[i];
			mShadowEffect.Render(mObject);
		}
		mShadowEffect.End();
	}

	//Draw scene
	{
		mSkybox.Render(mCamera);

		mStandardEffect.Begin();
		for (int i = 0; i < objCount; ++i)
		{
			mObject.transform.position = offsets[i];
			mStandardEffect.Render(mObject);
		}
		mStandardEffect.Render(mGround);
		mStandardEffect.End();
	}
}

void GameState::DebugUI()
{
	static Math::Vector3 tempLightDir = defaultLigthDir;

	ImGui::Begin("Debug Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Camera "), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Image(mShadowEffect.GetDepthMap().GetRawData(), { 150.0f, 150.0f });
	}

	ImGui::Separator();
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
	
	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Settings ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(" Use Specular Map ", &mStandardEffect.UseSpecularMap());
		ImGui::Checkbox(" Use Bump Map ", &mStandardEffect.UseDisplacementMap());
		ImGui::Checkbox(" Use Normal Map ", &mStandardEffect.UseNormalMap());
		ImGui::SliderFloat(" Bump Weight ", &mStandardEffect.DisplacementWeight(), 0.0f, 1.0f);
	}
	
	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Shadow ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat(" Depth Bias ", &mStandardEffect.DepthBias(), 0.000001f, 0.0f, 1.0f, "%.6f");
	}

	ImGui::End();
}
