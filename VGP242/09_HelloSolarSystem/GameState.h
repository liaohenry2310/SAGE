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
	Sage::Graphics::Skybox mSkybox;
	Sage::Graphics::MeshPX mSpaceshipMesh;
	Sage::Graphics::MeshPX mSphereMesh;
	Sage::Graphics::MeshPX mRingMesh;
	Sage::Graphics::MeshBuffer mSpaceshipMeshBuffer;
	Sage::Graphics::MeshBuffer mSphereMeshBuffer;
	Sage::Graphics::MeshBuffer mRingMeshBuffer;
	Sage::Graphics::VertexShader mVertexShader;
	Sage::Graphics::PixelShader mPixelShader;
	Sage::Graphics::Sampler mSampler;

	Sage::Graphics::Texture mSpaceshipTexture;
	Sage::Graphics::Texture mSunTexture;
	Sage::Graphics::Texture mMercuryTexture;
	Sage::Graphics::Texture mVenusTexture;
	Sage::Graphics::Texture mEarthTexture;
	Sage::Graphics::Texture mMoonTexture;
	Sage::Graphics::Texture mMarsTexture;
	Sage::Graphics::Texture mJupiterTexture;
	Sage::Graphics::Texture mSaturnTexture;
	Sage::Graphics::Texture mSaturnRingTexture;
	Sage::Graphics::Texture mUranusTexture;
	Sage::Graphics::Texture mNeptuneTexture;
	
	Sage::Graphics::RasterizerState mRasterizerStateWireframe;
	Sage::Graphics::RasterizerState mRasterizerStateNoCull;
	bool mUseWireframe = false;

	Sage::Graphics::DepthStencilState mDepthStencilStateDisableDepth;

	TransformBuffer mTransformBuffer;

	Sage::Math::Vector3 mShipPosition;
	Sage::Math::Vector3 mShipDirection;
	float mShipRotation = 0.0f;
	float mShipElevation = 0.0f;
	float mShipTilt = 0.0f;

	Sage::Math::Vector3 mCameraTargetPosition;
	float mCameraDistanceOffset = 15.0f;
	float mCameraHeightOffset = 3.0f;

	float mSunSpin = 0.0f;
	float mMercurySpin = 0.0f;
	float mMercuryOrbit = 0.0f;
	float mVenusSpin = 0.0f;
	float mVenusOrbit = 0.0f;
	float mEarthSpin = 0.0f;
	float mEarthOrbit = 0.0f;
	float mMoonSpin = 0.0f;
	float mMoonOrbit = 0.0f;
	float mMarsSpin = 0.0f;
	float mMarsOrbit = 0.0f;
	float mJupiterSpin = 0.0f;
	float mJupiterOrbit = 0.0f;
	float mSaturnSpin = 0.0f;
	float mSaturnOrbit = 0.0f;
	float mUranusSpin = 0.0f;
	float mUranusOrbit = 0.0f;
	float mNeptuneSpin = 0.0f;
	float mNeptuneOrbit = 0.0f;

	Sage::Graphics::ParticleEmitter mParticleEmitter;
};
