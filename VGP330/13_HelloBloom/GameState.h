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
	void RenderBrightness();
	void ApplyBlur();
	void PostProcess();

	struct TransformDataPX
	{
		Sage::Math::Matrix4 wvp;
	};

	struct PostProcessingData
	{
		int useWaveDisplacement = 0;
		int useRandomStripeDisplacement = 0;
		int useChromaticAberration = 0;
		int useScanLines = 0;
		float waveFrequency = 100.0f;
		float waveAmplitude = 0.005f;
		float stripesFrequency = 50.0f;
		float stripesThreshold = 0.95f;
		float stripeShiftAmount = 0.05f;
		float chromaticAberationAmount = 0.005f;
		float time = 0.0f;
		float padding;
	};

	struct BlurSettingsData
	{
		float screenWidth;
		float screenHeight;
		float multiplier;
		float padding;
	};

	using TransformBufferPX = Sage::Graphics::TypedConstantBuffer<TransformDataPX>;
	using PostProcessSettingsBuffer = Sage::Graphics::TypedConstantBuffer<PostProcessingData>;
	using BlurSettingsBuffer = Sage::Graphics::TypedConstantBuffer<BlurSettingsData>;

	Sage::Graphics::Camera mCamera;
	Sage::Graphics::Skybox mSkybox;

	Sage::Graphics::RenderTarget mBaseRenderTarget;
	Sage::Graphics::RenderTarget mBloomRenderTarget;
	Sage::Graphics::RenderTarget mBlurHRenderTarget;
	Sage::Graphics::RenderTarget mBlurVRenderTarget;

	Sage::Graphics::StandardEffect mStandardEffect;
	Sage::Graphics::RenderObject mEarth;
	Sage::Graphics::RenderObject mClouds;

	Sage::Graphics::MeshPX mScreenMesh;
	Sage::Graphics::MeshBuffer mScreenMeshBuffer;

	Sage::Graphics::MeshPX mSunMesh;

	Sage::Graphics::MeshBuffer mEarhMeshBuffer;
	Sage::Graphics::MeshBuffer mSunMeshBuffer;

	Sage::Graphics::VertexShader mPostProcessVertexShader;
	Sage::Graphics::PixelShader mPostProcessPixelShader;
	Sage::Graphics::VertexShader mUnlitVertexShader;
	Sage::Graphics::PixelShader mUnlitPixelShader;

	Sage::Graphics::VertexShader mGaussianBlurVertexShader;
	Sage::Graphics::PixelShader mGaussianBlurHPixelShader;
	Sage::Graphics::PixelShader mGaussianBlurVPixelShader;

	Sage::Graphics::BlendState mBlendState;
	Sage::Graphics::Sampler mSampler;

	TransformBufferPX mTransformBufferPX;

	PostProcessSettingsBuffer mPostProcessSettingsBuffer;
	BlurSettingsBuffer mBlurSettingsBuffer;

	TransformDataPX mTransformPXData;
	PostProcessingData mPostProcessSettingsData;
	BlurSettingsData mBlurSettingsData;

	Sage::Graphics::DirectionalLight mDirectionalLight;

	Sage::Graphics::Texture mCloudTexture;
	Sage::Graphics::Texture mSunTexture;

	Sage::Graphics::RasterizerState mRasterizerState;
	bool mUseWireframe = false;

	float mFPS = 0.0f;
	Sage::Math::Vector3 mEarthPosition = {2.0f, 0.0f, 0.0f};
	Sage::Math::Vector3 mEarthRotation = {0.0f, 0.0f, 0.0f};
	Sage::Math::Vector3 mEarthRevolution = {0.0f, 0.0f, 0.0f};

	int mBlurIteration = 10;
	float mBlurSaturation = 0.28f;
};