#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });
	mCamera.SetDirection(Vector3::ZAxis);

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

	//mMesh = MeshBuilder::CreateCubePX();
	mMesh = MeshBuilder::CreatePlanePX(3, 4, 0.5f);

	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

	mSamplerPoint.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mSamplerLinear.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mSamplerAnisotropic.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mTexture.Initialize(L"../../Assets/Images/mario_block.png");
}

void GameState::Terminate()
{
	mTexture.Terminate();
	mSamplerAnisotropic.Terminate();
	mSamplerLinear.Terminate();
	mSamplerPoint.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mTransformBuffer.Terminate();
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
}

// For homework:
// - Update the cube geometry so we have unique uvs per face (24v, 36i)
// - Find cool texture
// - Show three cubes, each drawn using different sampler to compare different the filtering modes
// - Add a MeshBuilder helper class in Graphics lib to keep the reuseable mesh building code
