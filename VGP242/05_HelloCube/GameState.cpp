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

	//Cube 1
	mMesh.vertices.emplace_back( Vector3{ -1.0f,  1.0f, -1.0f }, Color{ 1.0f, 1.0f, 0.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{  1.0f,  1.0f, -1.0f }, Color{ 1.0f, 1.0f, 0.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{  1.0f, -1.0f, -1.0f }, Color{ 0.0f, 1.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{ -1.0f, -1.0f, -1.0f }, Color{ 0.0f, 1.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{ -1.0f,  1.0f,  1.0f }, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{  1.0f,  1.0f,  1.0f }, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{  1.0f, -1.0f,  1.0f }, Color{ 1.0f, 1.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back( Vector3{ -1.0f, -1.0f,  1.0f }, Color{ 1.0f, 1.0f, 1.0f, 1.0f });

	//Cube 2
	mMesh.vertices.emplace_back(Vector3{ -1.0f,  1.0f, 2.0f }, Color{ 1.0f, 1.0f, 0.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{  1.0f,  1.0f, 2.0f }, Color{ 1.0f, 1.0f, 0.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{  1.0f, -1.0f, 2.0f }, Color{ 0.0f, 1.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{ -1.0f, -1.0f, 2.0f }, Color{ 0.0f, 1.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{ -1.0f,  1.0f, 4.0f }, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{  1.0f,  1.0f, 4.0f }, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{  1.0f, -1.0f, 4.0f }, Color{ 1.0f, 1.0f, 1.0f, 1.0f });
	mMesh.vertices.emplace_back(Vector3{ -1.0f, -1.0f, 4.0f }, Color{ 1.0f, 1.0f, 1.0f, 1.0f });

	//Cube 1
	//Front side
	mMesh.indices.emplace_back(8);
	mMesh.indices.emplace_back(9);
	mMesh.indices.emplace_back(10);
	mMesh.indices.emplace_back(10);
	mMesh.indices.emplace_back(11);
	mMesh.indices.emplace_back(8);
	//Back side
	mMesh.indices.emplace_back(13);
	mMesh.indices.emplace_back(12);
	mMesh.indices.emplace_back(15);
	mMesh.indices.emplace_back(15);
	mMesh.indices.emplace_back(14);
	mMesh.indices.emplace_back(13);
	//Top side
	mMesh.indices.emplace_back(8);
	mMesh.indices.emplace_back(12);
	mMesh.indices.emplace_back(13);
	mMesh.indices.emplace_back(13);
	mMesh.indices.emplace_back(9);
	mMesh.indices.emplace_back(8);
	//Bottom side
	mMesh.indices.emplace_back(11);
	mMesh.indices.emplace_back(10);
	mMesh.indices.emplace_back(14);
	mMesh.indices.emplace_back(14);
	mMesh.indices.emplace_back(10);
	mMesh.indices.emplace_back(9);
	//Right side
	mMesh.indices.emplace_back(9);
	mMesh.indices.emplace_back(13);
	mMesh.indices.emplace_back(14);
	mMesh.indices.emplace_back(14);
	mMesh.indices.emplace_back(15);
	mMesh.indices.emplace_back(11);
	//Left side
	mMesh.indices.emplace_back(12);
	mMesh.indices.emplace_back(8);
	mMesh.indices.emplace_back(11);
	mMesh.indices.emplace_back(11);
	mMesh.indices.emplace_back(15);
	mMesh.indices.emplace_back(12);


	//Cube 2
	//Front side
	mMesh.indices.emplace_back(0);
	mMesh.indices.emplace_back(1);
	mMesh.indices.emplace_back(2);
	mMesh.indices.emplace_back(2);
	mMesh.indices.emplace_back(3);
	mMesh.indices.emplace_back(0);
	//Back side
	mMesh.indices.emplace_back(5);
	mMesh.indices.emplace_back(4);
	mMesh.indices.emplace_back(7);
	mMesh.indices.emplace_back(7);
	mMesh.indices.emplace_back(6);
	mMesh.indices.emplace_back(5);
	//Top side
	mMesh.indices.emplace_back(0);
	mMesh.indices.emplace_back(4);
	mMesh.indices.emplace_back(5);
	mMesh.indices.emplace_back(5);
	mMesh.indices.emplace_back(1);
	mMesh.indices.emplace_back(0);
	//Bottom side
	mMesh.indices.emplace_back(3);
	mMesh.indices.emplace_back(2);
	mMesh.indices.emplace_back(6);
	mMesh.indices.emplace_back(6);
	mMesh.indices.emplace_back(2);
	mMesh.indices.emplace_back(1);
	//Right side
	mMesh.indices.emplace_back(1);
	mMesh.indices.emplace_back(5);
	mMesh.indices.emplace_back(6);
	mMesh.indices.emplace_back(6);
	mMesh.indices.emplace_back(7);
	mMesh.indices.emplace_back(3);
	//Left side
	mMesh.indices.emplace_back(4);
	mMesh.indices.emplace_back(0);
	mMesh.indices.emplace_back(3);
	mMesh.indices.emplace_back(3);
	mMesh.indices.emplace_back(7);
	mMesh.indices.emplace_back(4);

	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize(L"../../Assets/Shaders/DoTransform.fx", VertexPC::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTransform.fx");
}

void GameState::Terminate()
{
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
	auto matWorld = Matrix4::Identity;
	matWorld = matWorld * matWorld.RotationX(mRotation.x) * matWorld.RotationY(mRotation.y);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetProjectionMatrix();
	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mMeshBuffer.Render();
}

// For homework:
// - Add a ConstantBuffer class to the Graphics library and clean up HelloCube
// - Complete the section for mesh data so we are actually drawing a cube using indices
// - Add controls so we can rotate the cube by applying transformation matrices
// - Add multiple cubes, with one spin around the other
