#include "Precompiled.h"
#include "Skybox.h"

#include "MeshBuilder.h"

using namespace Sage::Graphics;
using namespace Sage::Math;

void Skybox::Initialize(const std::filesystem::path& filePath)
{
	mMesh = MeshBuilder::CreateSkyBox();

	mTransformBuffer.Initialize();

	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTexture.Initialize(filePath);

	mRasterizerStateWireFrame.Initialize(RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);
	mDepthStencilState.Initialize(false);
}

void Skybox::Render(const Camera& camera)
{
	ASSERT(!mMesh.vertices.empty(), "Skybox -- Not initialized!");

	mDepthStencilState.Set();
	mRasterizerStateWireFrame.Set();

	auto matWorld = Matrix4::Scaling(Vector3{ 0.1f }) * Matrix4::Translation(camera.GetPosition());
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetProjectionMatrix();
	Matrix4 data;

	data = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mTexture.BindPS(0);
	mMeshBuffer.Render();

	mDepthStencilState.Clear();
	mRasterizerStateWireFrame.Clear();
}

void Skybox::Terminate()
{
	mDepthStencilState.Terminate();
	mRasterizerStateWireFrame.Terminate();

	mTexture.Terminate();
	mSampler.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();

	mTransformBuffer.Terminate();
}
