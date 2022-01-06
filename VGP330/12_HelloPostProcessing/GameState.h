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
	void RenderScene();
	void PostProcess();

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
		float padding;
	};

	struct PostProcessingData
	{
		int useWaveDisplacement = 1;
		int useRandomStripeDisplacement = 1;
		int useChromaticAberration = 1;
		int useScanLines = 1;
		float waveFrequency = 100.0f;
		float waveAmplitude = 0.005f;
		float stripesFrequency = 50.0f;
		float stripesThreshold = 0.95f;
		float stripeShiftAmount = 0.05f;
		float chromaticAberationAmount = 0.005f;
		float time = 0.0f;
		float padding;
	};

	using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::DirectionalLight>;
	using MaterialBuffer = Sage::Graphics::TypedConstantBuffer<Sage::Graphics::Material>;
	using SettingsBuffer = Sage::Graphics::TypedConstantBuffer<SettingsData>;
	using PostProcessSettingsBuffer = Sage::Graphics::TypedConstantBuffer<PostProcessingData>;

	Sage::Graphics::Camera mCamera;

	Sage::Graphics::RenderTarget mRenderTarget;
	Sage::Graphics::MeshPX mScreenMesh;
	Sage::Graphics::MeshBuffer mScreenMeshBuffer;

	Sage::Graphics::Skybox mSkybox;

	Sage::Graphics::Mesh mEarthMesh;
	Sage::Graphics::MeshBuffer mEarhMeshBuffer;
	Sage::Graphics::Mesh mCloudsMesh;
	Sage::Graphics::MeshBuffer mCloudsMeshBuffer;

	Sage::Graphics::VertexShader mVertexShader;
	Sage::Graphics::PixelShader mPixelShader;

	Sage::Graphics::VertexShader mPostProcessVertexShader;
	Sage::Graphics::PixelShader mPostProcessPixelShader;

	Sage::Graphics::Sampler mSampler;
	Sage::Graphics::BlendState mBlendState;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;
	PostProcessSettingsBuffer mPostProcessSettingsBuffer;

	TransformData mTransformData;
	SettingsData mEarthSettingsData;
	SettingsData mCloudSettingsData;
	PostProcessingData mPostProcessSettingsData;

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