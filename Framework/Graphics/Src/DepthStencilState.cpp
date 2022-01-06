#include "Precompiled.h"
#include "DepthStencilState.h"

#include "GraphicsSystem.h"

using namespace Sage::Graphics;

DepthStencilState::~DepthStencilState()
{
	ASSERT(mDepthStencilState == nullptr, "DepthStencilState -- depth stencil state not released!");
}

void DepthStencilState::Initialize(bool depthEnable)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = depthEnable;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	ASSERT(SUCCEEDED(hr), "DepthStencilState -- Failed to create depth stencil state.");
}

void DepthStencilState::Terminate()
{
	SafeRelease(mDepthStencilState);
}

void DepthStencilState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(mDepthStencilState, 0);
}

void DepthStencilState::Clear()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(nullptr, 0);
}
