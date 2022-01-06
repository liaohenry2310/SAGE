#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace Sage;
using namespace Sage::Graphics;

void Sage::Graphics::Texture::UnbindPS(uint32_t slot)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "Texture -- Terminate must be called before destruction.");
}

Sage::Graphics::Texture::Texture(Texture&& rhs) noexcept : mShaderResourceView(rhs.mShaderResourceView)
{
	rhs.mShaderResourceView = nullptr;
}

Texture& Sage::Graphics::Texture::operator=(Texture&& rhs) noexcept
{
	mShaderResourceView = rhs.mShaderResourceView;
	rhs.mShaderResourceView = nullptr;
	return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Texture -- Failed to load texture %ls.", fileName.c_str());
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
	ASSERT(false, "Texture -- Not Implemented.");
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
	switch (format)
	{
	case Texture::Format::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case Texture::Format::RGBA_U32: return DXGI_FORMAT_R32G32B32A32_UINT;
	}

	return DXGI_FORMAT_R8G8B8A8_UNORM;
}
