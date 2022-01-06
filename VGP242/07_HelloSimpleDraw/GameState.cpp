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
	mTransformBuffer.Initialize();
	

	// Cube
	//
	//          4-----------------5
	//         /|                /|
	//        0-----------------1 |
	//	      | |               | |
	//	      | |               | |
	//	      | |               | |
	//	      | 7---------------|-6
	//	      |/                |/
	//	      3-----------------2

	mMesh = MeshBuilder::CreateCubePX();
	//mMesh = MeshBuilder::CreatePlanePX(3, 4, 0.5f);

	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

	mSamplerPoint.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mSamplerLinear.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mSamplerAnisotropic.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mTexture.Initialize(L"../../Assets/Images/mario_block.png");

	mRasterizerStateWireFrame.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);
}

void GameState::Terminate()
{
	mRasterizerStateWireFrame.Terminate();

	mTexture.Terminate();
	mSamplerAnisotropic.Terminate();
	mSamplerLinear.Terminate();
	mSamplerPoint.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mTransformBuffer.Terminate();

	mSkybox.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float moveSpeed = 10.0f;
	const float rotateSpeed = 50.0f * Constants::DegToRad;
	const float turnSpeed = 10.0f * Constants::DegToRad;
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

	auto matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetProjectionMatrix();
	TransformData data;

	data.wvp = Transpose(matWorld * Matrix4::Translation(Vector3{ -3.0f, 0.0f, 0.0f }) * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSamplerPoint.BindPS(0);
	mTexture.BindPS(0);
	mMeshBuffer.Render();

	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mSamplerLinear.BindPS(0);
	mMeshBuffer.Render();

	data.wvp = Transpose(matWorld * Matrix4::Translation(Vector3{ 3.0f, 0.0f, 0.0f }) * matView * matProj);
	mTransformBuffer.Update(data);
	mSamplerAnisotropic.BindPS(0);
	mMeshBuffer.Render();

	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::AddAABB({ 2.0f, 2.0f, 2.0f }, { 1.0f, 1.0f, 0.2f }, Colors::Blue, true);
	SimpleDraw::AddGroundPlane(20);
	SimpleDraw::AddSphere({ -2.0f, 0.5f, 2.0f }, 0.5f, Colors::Red, 16, 16);
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
	static int renderModeIndex = 0;
	if (ImGui::Combo("Render Mode", &renderModeIndex, renderModeItems, IM_ARRAYSIZE(renderModeItems)))
	{
		mUseWireframe = (renderModeIndex != 0);
	}

	ImGui::End();

	//ImGui::ShowDemoWindow();
}

// Homework
// - Add a skybox class to Graphics
// - Use DoTexturing.fx shader (you can make a copy of it and call it skybox.fx)
// - Create a textured cube mesh for drawing
//		- Have a MeshBuilder::CreateSkybox function that generates a box that is textured mapped on the inside
// - For drawing, make sure the skybox is the same position as the camera (but leave rotation unchanged)
// - Make sure it is drawn first before anything else in your scene, disable Z when drawing the skybox
