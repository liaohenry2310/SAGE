#pragma once

namespace Sage::Graphics
{
	class MeshBuffer
	{
	public:
		enum class Topology { Points, Lines, Triangles };

		MeshBuffer() = default;
		~MeshBuffer();

		MeshBuffer(const MeshBuffer&) = delete;
		MeshBuffer& operator=(const MeshBuffer&) = delete;

		MeshBuffer(MeshBuffer&& rhs) noexcept;
		MeshBuffer& operator=(MeshBuffer&& rhs) noexcept;

		template <class MeshT>
		void Initialize(const MeshT& mesh, bool dynamic = false)
		{
			Initialize(mesh.vertices.data(), 
				static_cast<uint32_t>(sizeof(MeshT::VertexType)), 
				static_cast<uint32_t>(mesh.vertices.size()), 
				mesh.indices.data(),
				static_cast<uint32_t>(mesh.indices.size()),
				dynamic);
		}

		void Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, bool dynamic = false);
		void Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, const uint32_t* indexData, uint32_t numIndices, bool dynamic = false);
		void Terminate();

		void SetTopology(Topology topology);

		void Update(const void* verticies, uint32_t numVerticies);

		void Render() const;

	private:
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;

		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		uint32_t mVertexSize = 0;
		uint32_t mVertexCount = 0;
		uint32_t mIndexCount = 0;
	};
}