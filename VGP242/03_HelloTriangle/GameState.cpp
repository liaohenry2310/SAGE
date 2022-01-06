#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	// NDC - Normalized Device Coordinate
	//
	//          +-----------------+
	//         /                 /|
	//   1.0  +-----------------+ |
	//	      |        ^        | |
	//	      |        |        | |
	//	      | <------|------> | |
	//	      |        |        | | 1.0
	//	      |        v        |/
	//	-1.0  +-----------------+ 0.0
	//      -1.0               1.0
	//
	mMesh.vertices.emplace_back( Vector3{ 0.0f,  0.5f, 0.0f }, Color{1.0f, 1.0f, 0.0f, 1.0f});
	mMesh.vertices.emplace_back( Vector3{ 0.5f, -0.5f, 0.0f }, Color{1.0f, 1.0f, 0.0f, 1.0f});
	mMesh.vertices.emplace_back( Vector3{-0.5f, -0.5f, 0.0f }, Color{1.0f, 1.0f, 0.0f, 1.0f});
	mMesh.vertices.emplace_back( Vector3{ 0.5f,  0.0f, 0.0f }, Color{1.0f, 1.0f, 0.0f, 1.0f});

	mMesh.indices.emplace_back(0);
	mMesh.indices.emplace_back(1);
	mMesh.indices.emplace_back(2);
	mMesh.indices.emplace_back(0);
	mMesh.indices.emplace_back(3);
	mMesh.indices.emplace_back(1);

	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize(L"../../Assets/Shaders/DoNothing.fx", VertexPC::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoNothing.fx");
}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();
}