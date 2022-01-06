#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f, 0.0f, -25.0f });
	mCamera.SetDirection(Vector3::ZAxis);

	mSkybox.Initialize(L"../../Assets/Images/skybox.jpg");
	mRasterizerStateWireFrame.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);
}

void GameState::Terminate()
{
	mRasterizerStateWireFrame.Terminate();
	mSkybox.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float moveSpeed = 10.0f;
	const float rotateSpeed = 100.0f * Constants::DegToRad;
	const float turnSpeed = 30.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mRotation.x += rotateSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mRotation.x += -rotateSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mRotation.y += -rotateSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mRotation.y += rotateSpeed * deltaTime;
	}
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}


void GameState::Render()
{
	mSkybox.Render(mCamera);
	mUseWireframe ? mRasterizerStateWireFrame.Set() : mRasterizerStateWireFrame.Clear();

	SimpleDraw::AddTransform({ Matrix4::Identity * Matrix4::Translation({2.0f, 4.0f, 0.0f}) });
	SimpleDraw::AddGroundPlane(20);

	//Head
	SimpleDraw::AddSphere({ 0.0f, 5.0f, 0.0f }, 1.0f, Colors::LightCoral, 16, 16);
	SimpleDraw::AddSphere({ 0.5f, 5.0f, -1.0f }, 0.15f, Colors::Black, 8, 8);
	SimpleDraw::AddSphere({ -0.5f, 5.0f, -0.9f }, 0.15f, Colors::Black, 8, 8);
	//Hat
	SimpleDraw::AddCone({ 0.15f, 5.8f, 0.15f }, { 0.2f, 1.0f, 0.2f }, 0.6f, 1.5f, Colors::Blue, 8, 16);
	//Body
	SimpleDraw::AddCapsule({ 0.0f, 3.0f, 0.0f }, 0.6f, 2.0f, Colors::Blue, 8, 16);
	//Arms
	SimpleDraw::AddCylinder({ 0.3f, 4.0f, 0.0f }, { 1.0f, 0.7f, 0.0f }, 0.2f, 2.0f, Colors::LightCoral, 4, 8);
	SimpleDraw::AddCylinder({ -0.3f, 4.0f, 0.0f }, { -1.0f, -0.7f, 0.5f }, 0.2f, 2.0f, Colors::LightCoral, 4, 8);
	//Legs
	SimpleDraw::AddCylinder({ 0.3f, 2.0f, 0.0f }, { 0.5f, -1.0f, 0.5f }, 0.2f, 2.0f, Colors::Blue, 4, 8);
	SimpleDraw::AddCylinder({ -0.3f, 2.0f, 0.0f }, { -0.5f, -1.0f, -0.7f }, 0.2f, 2.0f, Colors::Blue, 4, 8);

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//UI theme
	const char* themeItems[] = { "Classic", "Dark", "Light" };
	static int themeItemIndex = 0;
	if (ImGui::Combo("UI Theme", &themeItemIndex, themeItems, IM_ARRAYSIZE(themeItems)))
	{
		if (themeItemIndex == 0)
			DebugUI::SetTheme(DebugUI::Theme::Classic);
		else if (themeItemIndex == 1)
			DebugUI::SetTheme(DebugUI::Theme::Dark);
		else
			DebugUI::SetTheme(DebugUI::Theme::Light);
	}

	//Render mode
	const char* renderModeItems[] = { "Solid", "Wireframe"};
	static int renderModeIndex = 1;
	if (ImGui::Combo("Render Mode", &renderModeIndex, renderModeItems, IM_ARRAYSIZE(renderModeItems)))
	{
		mUseWireframe = (renderModeIndex != 0);
	}

	ImGui::End();
}
