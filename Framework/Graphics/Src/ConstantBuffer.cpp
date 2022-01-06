#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace Sage::Graphics;
using namespace Sage::Math;

void ConstantBuffer::UnBindVS(uint32_t slot)
{
	static ID3D11Buffer* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->VSSetConstantBuffers(slot, 1, &dummy);
}

void ConstantBuffer::UnBindPS(uint32_t slot)
{
	static ID3D11Buffer* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetConstantBuffers(slot, 1, &dummy);
}

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "Constant Buffer -- Constant buffer not released.");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer.");
}

void ConstantBuffer::Update(const void* data) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void Sage::Graphics::ConstantBuffer::BindVS(uint32_t slot)
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void Sage::Graphics::ConstantBuffer::BindPS(uint32_t slot)
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}
