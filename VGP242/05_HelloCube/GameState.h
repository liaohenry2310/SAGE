#pragma once
#include <Sage/Inc/Sage.h>

class GameState : public Sage::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

private:
	struct TransformData
	{
		Sage::Math::Matrix4 wvp;
	};

	using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;

	Sage::Graphics::Camera mCamera;
	Sage::Graphics::MeshPC mMesh;
	Sage::Graphics::MeshBuffer mMeshBuffer;
	Sage::Graphics::VertexShader mVertexShader;
	Sage::Graphics::PixelShader mPixelShader;

	TransformBuffer mTransformBuffer;
	
	Sage::Math::Vector3 mRotation;
};