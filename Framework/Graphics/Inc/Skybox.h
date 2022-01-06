#pragma once

#include "Camera.h"
#include "DepthStencilState.h"
#include "MeshBuffer.h"
#include "MeshTypes.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "Sampler.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"

namespace Sage::Graphics
{
	class Skybox
	{
	public:
		Skybox() = default;
		~Skybox() = default;

		Skybox(const Skybox&) = delete;
		Skybox& operator=(const Skybox&) = delete;

		void Initialize(const std::filesystem::path& filePath);
		void Render(const Camera& camera);
		void Terminate();

	private:
		MeshPX mMesh;
		Texture mTexture;
		MeshBuffer mMeshBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		RasterizerState mRasterizerStateWireFrame;
		DepthStencilState mDepthStencilState;

		TypedConstantBuffer<Sage::Math::Matrix4> mTransformBuffer;
	};
}