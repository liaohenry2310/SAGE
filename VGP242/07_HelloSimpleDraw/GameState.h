#pragma once
#include <Sage/Inc/Sage.h>

class GameState : public Sage::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct TransformData
	{
		Sage::Math::Matrix4 wvp;
	};

	using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;

	Sage::Graphics::Camera mCamera;
	Sage::Graphics::MeshPX mMesh;
	Sage::Graphics::MeshBuffer mMeshBuffer;
	Sage::Graphics::VertexShader mVertexShader;
	Sage::Graphics::PixelShader mPixelShader;
	Sage::Graphics::Sampler mSamplerPoint;
	Sage::Graphics::Sampler mSamplerLinear;
	Sage::Graphics::Sampler mSamplerAnisotropic;
	Sage::Graphics::Texture mTexture;

	Sage::Graphics::RasterizerState mRasterizerStateWireFrame;
	bool mUseWireframe = false;
	
	TransformBuffer mTransformBuffer;
	Sage::Math::Vector3 mRotation;

	Sage::Graphics::Skybox mSkybox;
};