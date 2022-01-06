#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

namespace
{
	const Math::Vector3 defaultLigthDir = { 10.0f, -10.0f, 10.0f };
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.0f, -1.25f });
	mCamera.SetDirection(Vector3::ZAxis);

	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferWidth(), RenderTarget::Format::RGBA_U8);

	mScreenMesh.vertices.push_back({ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	mScreenMesh.vertices.push_back({ { -1.0f, +1.0f, 0.0f }, { 0.0f, 0.0f } });
	mScreenMesh.vertices.push_back({ { +1.0f, +1.0f, 0.0f }, { 1.0f, 0.0f } });
	mScreenMesh.vertices.push_back({ { +1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });
	mScreenMesh.indices = { 0, 1, 2, 0, 2, 3 };
	mScreenMeshBuffer.Initialize(mScreenMesh);

	mSkybox.Initialize("../../Assets/Images/space.jpg");

	mEarthMesh = MeshBuilder::CreatSphere(512, 512);
	mEarhMeshBuffer.Initialize(mEarthMesh);

	mCloudsMesh = MeshBuilder::CreatSphere(32, 32);
	mCloudsMeshBuffer.Initialize(mCloudsMesh);

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mPostProcessVertexShader.Initialize("../../Assets/Shaders/PostProcess.fx", VertexPX::Format);
	mPostProcessPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::Additive);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mPostProcessSettingsBuffer.Initialize();

	mDiffuseMap.Initialize("../../Assets/Images/earth.jpg");
	mSpecularMap.Initialize("../../Assets/Images/earth_spec.jpg");
	mBumpMap.Initialize("../../Assets/Images/earth_bump.jpg");
	mNormalMap.Initialize("../../Assets/Images/earth_normal.jpg");
	mCloudTexture.Initialize("../../Assets/Images/earth_clouds.jpg");

	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);

	//Setting default lighting and material values.
	mLightData.lightDirection = Normalize(defaultLigthDir);
	mLightData.ambient = Graphics::Colors::Black;
	mLightData.diffuse = Graphics::Colors::White;
	mLightData.specular = Graphics::Colors::Gray;

	mMaterialData.ambient = Graphics::Colors::White;
	mMaterialData.diffuse = Graphics::Colors::White;
	mMaterialData.specular = Graphics::Colors::White;
	mMaterialData.power = 10.0f;

	mCloudSettingsData.useBumpMap = 0;
	mCloudSettingsData.useSpecularMap = 0;
}

void GameState::Terminate()
{
	mRasterizerState.Terminate();

	mCloudTexture.Terminate();
	mNormalMap.Terminate();
	mBumpMap.Terminate();
	mSpecularMap.Terminate();
	mDiffuseMap.Terminate();

	mPostProcessSettingsBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	mBlendState.Terminate();
	mSampler.Terminate();

	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mCloudsMeshBuffer.Terminate();
	mEarhMeshBuffer.Terminate();

	mSkybox.Terminate();

	mScreenMeshBuffer.Terminate();

	mRenderTarget.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float moveSpeed = 2.5f;
	const float rotateSpeed = 100.0f * Constants::DegToRad;
	const float turnSpeed = 30.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	mFPS = 1.0f / deltaTime;
	mEarthSpin += 0.3f * deltaTime;
	mCloudSpin += 0.2f * deltaTime;

	mPostProcessSettingsData.time += deltaTime;
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	RenderScene();
	mRenderTarget.EndRender();

	PostProcess();
}

void GameState::DebugUI()
{
	static Math::Vector3 tempLightDir = defaultLigthDir;

	ImGui::Begin("Debug Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Lighting ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::SliderFloat3(" Light Direction (x, y, z) ", &tempLightDir.x, -10.0f, 10.0f))
		{
			mLightData.lightDirection = Math::Normalize(tempLightDir);
		}
		ImGui::ColorEdit4(" Light Ambient ", &mLightData.ambient.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Diffuse ", &mLightData.diffuse.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Specular ", &mLightData.specular.r, ImGuiColorEditFlags_NoAlpha);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::ColorEdit4(" Material Ambient ", &mMaterialData.ambient.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Material Diffuse ", &mMaterialData.diffuse.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Material Specular ", &mMaterialData.specular.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::SliderFloat(" Specular Power ", &mMaterialData.power, 0.0f, 20.0f);
	}

	if (ImGui::CollapsingHeader(" Settings "), ImGuiTreeNodeFlags_DefaultOpen)
	{
		bool useSpecularFlag = mEarthSettingsData.useSpecularMap == 1;
		bool useBumbFlag = mEarthSettingsData.useBumpMap == 1;

		if (ImGui::Checkbox(" Use Specular Map ", &useSpecularFlag))
		{
			mEarthSettingsData.useSpecularMap = useSpecularFlag ? 1 : 0;
		}
		if (ImGui::Checkbox(" Use Bump Map ", &useBumbFlag))
		{
			mEarthSettingsData.useBumpMap = useBumbFlag ? 1 : 0;
		}
		ImGui::SliderFloat(" Bump Weight ", &mEarthSettingsData.bumbWeight, 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader(" Post Processing "), ImGuiTreeNodeFlags_DefaultOpen)
	{
		bool useWaveDisplacement = mPostProcessSettingsData.useWaveDisplacement == 1;
		bool useRandomStripeDisplacement = mPostProcessSettingsData.useRandomStripeDisplacement == 1;
		bool useChromaticAberration = mPostProcessSettingsData.useChromaticAberration == 1;
		bool useScanLines = mPostProcessSettingsData.useScanLines == 1;

		static float stripeThickess = 50.0f;

		if (ImGui::Checkbox(" Use Wave Displacement ", &useWaveDisplacement))
		{
			mPostProcessSettingsData.useWaveDisplacement = useWaveDisplacement;
		}
		if (ImGui::Checkbox(" Use Random Stripes Displacement ", &useRandomStripeDisplacement))
		{
			mPostProcessSettingsData.useRandomStripeDisplacement = useRandomStripeDisplacement;
		}
		if (ImGui::Checkbox(" Use Chromatic Aberration ", &useChromaticAberration))
		{
			mPostProcessSettingsData.useChromaticAberration = useChromaticAberration;
		}
		if (ImGui::Checkbox(" Use Scan Lines ", &useScanLines))
		{
			mPostProcessSettingsData.useScanLines = useScanLines;
		}

		ImGui::SliderFloat(" Wave Frequency ", &mPostProcessSettingsData.waveFrequency, 0.0f, 100.0f);
		ImGui::SliderFloat(" Wave Amplitude ", &mPostProcessSettingsData.waveAmplitude, 0.0f, 0.1f);
		if (ImGui::SliderFloat(" Stripes Thickness ", &stripeThickess, 1.0f, 100.0f))
		{
			mPostProcessSettingsData.stripesFrequency = 101.0f - stripeThickess;
		}
		ImGui::SliderFloat(" Stripes Threshold ", &mPostProcessSettingsData.stripesThreshold, 0.5f, 1.0f);
		ImGui::SliderFloat(" Stripes Shift Amount ", &mPostProcessSettingsData.stripeShiftAmount, 0.0f, 1.0f);
		ImGui::SliderFloat(" Chromatic Aberration Amount ", &mPostProcessSettingsData.chromaticAberationAmount, 0.0f, 0.1f);
	}
	
	ImGui::End();
}

void GameState::RenderScene()
{
	mSkybox.Render(mCamera);

	mVertexShader.Bind();
	mPixelShader.Bind();

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetProjectionMatrix();

	//Render Earth
	Matrix4 matWorld = Matrix4::RotationY(mEarthSpin);
	mTransformData.world = Transpose(matWorld);
	mTransformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.Update(mTransformData);
	mLightBuffer.Update(mLightData);
	mMaterialBuffer.Update(mMaterialData);
	mSettingsBuffer.Update(mEarthSettingsData);

	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mDiffuseMap.BindPS(0);
	mSpecularMap.BindPS(1);
	mBumpMap.BindVS(2);
	mNormalMap.BindPS(3);

	mEarhMeshBuffer.Render();

	//Render Clouds
	mBlendState.Set();
	float scale = mEarthSettingsData.bumbWeight;
	matWorld = Matrix4::Scaling({ 1.0f + 1.7f * scale, 1.0f + 1.7f * scale, 1.0f + 1.7f * scale }) * Matrix4::RotationY(mCloudSpin);
	mTransformData.world = Transpose(matWorld);
	mTransformData.wvp = Transpose(matWorld * matView * matProj);

	Vector4 s = mMaterialData.specular;
	mMaterialData.specular = { 0.0f, 0.0f, 0.0f, 0.0f };

	mTransformBuffer.Update(mTransformData);
	mSettingsBuffer.Update(mCloudSettingsData);
	mMaterialBuffer.Update(mMaterialData);

	mMaterialData.specular = s;

	mTransformBuffer.BindVS(0);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);
	mCloudTexture.BindPS(0);

	mCloudsMeshBuffer.Render();
	mBlendState.ClearState();
}

void GameState::PostProcess()
{
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	mPostProcessSettingsBuffer.Update(mPostProcessSettingsData);
	mPostProcessSettingsBuffer.BindPS(0);
	mSampler.BindPS(0);

	mRenderTarget.BindPS(0);
	mScreenMeshBuffer.Render();
	Texture::UnbindPS(0);
}
