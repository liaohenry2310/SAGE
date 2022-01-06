#include "Precompiled.h"
#include "MeshBuffer.h"

#include "GraphicsSystem.h"

using namespace Sage::Graphics;

MeshBuffer::~MeshBuffer()
{
	ASSERT(mVertexBuffer == nullptr, "MeshBuffer -- Vertex buffer not released!");
	ASSERT(mIndexBuffer == nullptr, "MeshBuffer -- Index buffer not released!");
}

Sage::Graphics::MeshBuffer::MeshBuffer(MeshBuffer&& rhs) noexcept
	: mVertexBuffer(rhs.mVertexBuffer)
	, mIndexBuffer(rhs.mIndexBuffer)
	, mTopology(rhs.mTopology)
	, mVertexSize(rhs.mVertexSize)
	, mVertexCount(rhs.mVertexCount)
	, mIndexCount(rhs.mIndexCount)
{
	rhs.mVertexBuffer = nullptr;
	rhs.mIndexBuffer = nullptr;
}

MeshBuffer& Sage::Graphics::MeshBuffer::operator=(MeshBuffer&& rhs) noexcept
{
	if (this != &rhs)
	{
		mVertexBuffer = rhs.mVertexBuffer;
		mIndexBuffer = rhs.mIndexBuffer;
		mTopology = rhs.mTopology;
		mVertexSize = rhs.mVertexSize;
		mVertexCount = rhs.mVertexCount;
		mIndexCount = rhs.mIndexCount;
		rhs.mVertexBuffer = nullptr;
		rhs.mIndexBuffer = nullptr;
	}
	return *this;
}

void MeshBuffer::Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, bool dynamic)
{
	Initialize(vertexData, vertexSize, numVertices, nullptr, 0, dynamic);
}

void MeshBuffer::Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, const uint32_t* indexData, uint32_t numIndices, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = numVertices;
	mIndexCount = numIndices;

	// Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = numVertices * vertexSize;
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertexData;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&bufferDesc, vertexData? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	if (indexData && numIndices > 0)
	{
		bufferDesc.ByteWidth = numIndices * sizeof(uint32_t);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indexData;
		hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
		ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");
	}
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Sage::Graphics::MeshBuffer::Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case Sage::Graphics::MeshBuffer::Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Sage::Graphics::MeshBuffer::Topology::Triangles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
}

void MeshBuffer::Update(const void* vertices, uint32_t numVertices)
{
	mVertexCount = numVertices;

	auto context = GraphicsSystem::Get()->GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, numVertices * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::Render() const
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetPrimitiveTopology(mTopology);

	UINT stride = (UINT)mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	if (mIndexBuffer)
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		context->Draw((UINT)mVertexCount, 0);
	}
}
