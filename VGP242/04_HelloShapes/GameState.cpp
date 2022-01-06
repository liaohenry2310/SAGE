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
	mVerticesArray[0].push_back(Vertex{ Vector3{ -0.5f, 0.1f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ 0.5f, 0.1f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ 0.0f, -0.5f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ -0.5f, 0.1f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ -0.25f, 0.35f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ 0.0f, 0.1f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ 0.0f, 0.1f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ 0.25f, 0.35f, 0.0f }, Colors::Red });
	mVerticesArray[0].push_back(Vertex{ Vector3{ 0.5f, 0.1f, 0.0f }, Colors::Red });
	
	mVerticesArray[1].push_back(Vertex{ Vector3{ 0.0f, 0.4f, 0.0f}, Colors::LightGoldenrodYellow });
	mVerticesArray[1].push_back(Vertex{ Vector3{ 0.2f, 0.0f, 0.0f}, Colors::Goldenrod });
	mVerticesArray[1].push_back(Vertex{ Vector3{ -0.2f, 0.0f, 0.0f}, Colors::DarkGoldenrod });
	mVerticesArray[1].push_back(Vertex{ Vector3{ -0.2f, 0.0f, 0.0f}, Colors::LightGoldenrodYellow });
	mVerticesArray[1].push_back(Vertex{ Vector3{ 0.0f, -0.4f, 0.0f}, Colors::Goldenrod });
	mVerticesArray[1].push_back(Vertex{ Vector3{ -0.4f, -0.4f, 0.0f}, Colors::DarkGoldenrod });
	mVerticesArray[1].push_back(Vertex{ Vector3{ 0.2f, 0.0f, 0.0f }, Colors::LightGoldenrodYellow });
	mVerticesArray[1].push_back(Vertex{ Vector3{ 0.4f, -0.4f, 0.0f}, Colors::Goldenrod });
	mVerticesArray[1].push_back(Vertex{ Vector3{ 0.0f, -0.4f, 0.0f}, Colors::DarkGoldenrod });

	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.0f, 0.0f, 0.0f}, Colors::Gold });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.075f, -0.075f, 0.0f}, Colors::Gold });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.0, -0.15f, 0.0f}, Colors::Gold });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.0f, 0.0f, 0.0f}, Colors::Gold });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.0, -0.15f, 0.0f}, Colors::Gold });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.075f, -0.075f, 0.0f}, Colors::Gold });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.1f, 0.0f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.1f, 0.7f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.1f, 0.7f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.1f, 0.7f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.1f, 0.0f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.1f, 0.0f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.1f, 0.7f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.0f, 0.85f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.1f, 0.7f, 0.0f}, Colors::White });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.3f, 0.0f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.3f, 0.0f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.2f, -0.15f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.3f, 0.0f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.2f, -0.15f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.2f, -0.15f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.05f, -0.15f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.05f, -0.15f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.05f, -0.5f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.05f, -0.15f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.05f, -0.5f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.05f, -0.5f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.05f, -0.5f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.05f, -0.5f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.1f, -0.6f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.05f, -0.5f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ 0.1f, -0.6f, 0.0f}, Colors::DarkBlue });
	mVerticesArray[2].push_back(Vertex{ Vector3{ -0.1f, -0.6f, 0.0f}, Colors::DarkBlue });

	auto device = GraphicsSystem::Get()->GetDevice();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(mVerticesArray[0].size() * sizeof(Vertex));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVerticesArray[0].data();

	//Create vertex buffers
	HRESULT hr;
	for (size_t i = 0; i < mVerticesArray.size(); ++i)
	{
		bufferDesc.ByteWidth = static_cast<UINT>(mVerticesArray[i].size() * sizeof(Vertex));
		initData.pSysMem = mVerticesArray[i].data();
		hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffers[i]);
		ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
	}

	// Compile and create vertex shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	// Define vertex element descriptions
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	// Create the input layout
	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void GameState::Terminate()
{
	for (int i = 0; i < mVertexBuffers.size(); ++i)
	{
		SafeRelease(mVertexBuffers[i]);
	}
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void GameState::Update(float deltatime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		mCurrentBuffer = (mCurrentBuffer + 1) % static_cast<int>(mVertexBuffers.size());
	}
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffers[mCurrentBuffer], &stride, &offset);

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw((UINT)mVerticesArray[mCurrentBuffer].size(), 0);
}

// Add 04_HelloShapes project
// - Copy code from 03_HelloTriangle
// - Update to use DoSomething.fx shaders
// - Update Vertex to have Color as well
// - Make sure inputlayout is initialized with correct description
// - Create multiple meshes and vertex buffers
// - Draw a heart
// - Draw a triforce
// - Draw something with more than 5 triangles
// - Use input check so you can change between them

