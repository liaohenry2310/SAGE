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
		Sage::Math::Matrix4 world;
		Sage::Math::Matrix4 wvp;
		Sage::Math::Vector3 viewPosition;
		float padding;
	};

	struct SettingsData
	{
		int useSpecularMap = 1;
		int useBumpMap = 1;
		float bumbWeight = 0.1f;
		float padding = 0.0f;
	};

	using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::DirectionalLight>;
	using MaterialBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::Material>;
	using SettingsBuffer = Sage::Graphics::TypedConstantBuffer<SettingsData>;

	Sage::Graphics::Camera mCamera;
	Sage::Graphics::Skybox mSkybox;

	Sage::Graphics::Mesh mEarthMesh;
	Sage::Graphics::MeshBuffer mEarhMeshBuffer;

	Sage::Graphics::Mesh mCloudsMesh;
	Sage::Graphics::MeshBuffer mCloudsMeshBuffer;

	Sage::Graphics::VertexShader mVertexShader;
	Sage::Graphics::PixelShader mPixelShader;
	Sage::Graphics::Sampler mSampler;
	Sage::Graphics::BlendState mBlendState;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;

	TransformData mTransformData;
	SettingsData mEarthSettingsData;
	SettingsData mCloudSettingsData;

	Sage::Graphics::DirectionalLight mLightData;
	Sage::Graphics::Material mMaterialData;
	Sage::Graphics::Texture mDiffuseMap;
	Sage::Graphics::Texture mSpecularMap;
	Sage::Graphics::Texture mBumpMap;
	Sage::Graphics::Texture mNormalMap;
	Sage::Graphics::Texture mCloudTexture;

	Sage::Graphics::RasterizerState mRasterizerState;
	bool mUseWireframe = false;

	float mFPS = 0.0f;
	float mEarthSpin = 0.0f;
	float mCloudSpin = 0.0f;
};