#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

namespace
{
	const Math::Vector3 defaultLigthDir = { 10.0f, 0.0f, 0.0f };
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.0f, -3.25f });
	mCamera.SetDirection(Vector3::ZAxis);

	mSkybox.Initialize("../../Assets/Images/space.jpg");

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);

	mDirectionalLight.lightDirection = Normalize(defaultLigthDir);
	mDirectionalLight.ambient = Graphics::Colors::Black;
	mDirectionalLight.diffuse = Graphics::Colors::White;
	mDirectionalLight.specular = Graphics::Colors::Gray;
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mEarth.material.ambient = Graphics::Colors::White;
	mEarth.material.diffuse = Graphics::Colors::White;
	mEarth.material.specular = Graphics::Colors::White;
	mEarth.material.power = 10.0f;
	mEarth.diffuseMap.Initialize("../../Assets/Images/earth.jpg");
	mEarth.specularMap.Initialize("../../Assets/Images/earth_spec.jpg");
	mEarth.bumpMap.Initialize("../../Assets/Images/earth_bump.jpg");
	mEarth.normalMap.Initialize("../../Assets/Images/earth_normal.jpg");
	mEarth.meshBuffer.Initialize(MeshBuilder::CreatSphere(512, 512));

	mClouds.material.ambient = Graphics::Colors::White;
	mClouds.material.diffuse = Graphics::Colors::White;
	mClouds.material.specular = Graphics::Colors::Black;
	mClouds.material.power = 10.0f;
	mClouds.diffuseMap.Initialize("../../Assets/Images/earth_clouds.jpg");
	mClouds.meshBuffer.Initialize(MeshBuilder::CreatSphere(512, 512));

	mSunTexture.Initialize("../../Assets/Images/sun.jpg");
	mSunMesh = MeshBuilder::CreateSpherePX(32, 32);
	mSunMeshBuffer.Initialize(mSunMesh);

	mScreenMesh.vertices.push_back({ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	mScreenMesh.vertices.push_back({ { -1.0f, +1.0f, 0.0f }, { 0.0f, 0.0f } });
	mScreenMesh.vertices.push_back({ { +1.0f, +1.0f, 0.0f }, { 1.0f, 0.0f } });
	mScreenMesh.vertices.push_back({ { +1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });
	mScreenMesh.indices = { 0, 1, 2, 0, 2, 3 };
	mScreenMeshBuffer.Initialize(mScreenMesh);

	auto graphicsSystem = GraphicsSystem::Get();
	const auto screenWidth = graphicsSystem->GetBackBufferWidth();
	const auto screenHeight = graphicsSystem->GetBackBufferHeight();
	mBaseRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mBloomRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mBlurHRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mBlurVRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mPostProcessVertexShader.Initialize("../../Assets/Shaders/PostProcess.fx", VertexPX::Format);
	mPostProcessPixelShader.Initialize("../../Assets/Shaders/PostProcess.fx");

	mUnlitVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mUnlitPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");

	mGaussianBlurVertexShader.Initialize("../../Assets/Shaders/GaussianBlur.fx", VertexPX::Format);
	mGaussianBlurHPixelShader.Initialize("../../Assets/Shaders/GaussianBlur.fx", "BlurXPS");
	mGaussianBlurVPixelShader.Initialize("../../Assets/Shaders/GaussianBlur.fx", "BlurYPS");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::Additive);

	mEarhMeshBuffer.Initialize(MeshBuilder::CreatSphere(512, 512));

	mTransformBufferPX.Initialize();

	mPostProcessSettingsBuffer.Initialize();

	mBlurSettingsBuffer.Initialize();

	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);


}

void GameState::Terminate()
{
	mRasterizerState.Terminate();

	mSunTexture.Terminate();
	mCloudTexture.Terminate();

	mBlurSettingsBuffer.Terminate();
	mPostProcessSettingsBuffer.Terminate();

	mTransformBufferPX.Terminate();
	
	mStandardEffect.Terminate();

	mBlendState.Terminate();
	mSampler.Terminate();

	mGaussianBlurVertexShader.Terminate();
	mGaussianBlurHPixelShader.Terminate();
	mGaussianBlurVPixelShader.Terminate();

	mUnlitPixelShader.Terminate();
	mUnlitVertexShader.Terminate();
	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();

	mSunMeshBuffer.Terminate();

	mEarhMeshBuffer.Terminate();
	mClouds.Terminate();
	mEarth.Terminate();

	mSkybox.Terminate();

	mScreenMeshBuffer.Terminate();

	mBlurVRenderTarget.Terminate();
	mBlurHRenderTarget.Terminate();
	mBloomRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();
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
	
	mEarth.transform.position = Math::TransformCoord(mEarthPosition, Matrix4::RotationY(mEarthRevolution.y));
	mEarth.transform.rotation = Quaternion::RotationEuler(mEarthRotation);

	mClouds.transform.position = mEarth.transform.position;
	mClouds.transform.rotation = mEarth.transform.rotation;
	mClouds.transform.scale = { 1.2f, 1.2f , 1.2f };

	mPostProcessSettingsData.time += deltaTime;
}

void GameState::Render()
{
	mBaseRenderTarget.BeginRender();
	RenderScene();
	mBaseRenderTarget.EndRender();

	mBloomRenderTarget.BeginRender();
	RenderBrightness();
	mBloomRenderTarget.EndRender();

	ApplyBlur();
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
			mDirectionalLight.lightDirection = Math::Normalize(tempLightDir);
		}
		ImGui::ColorEdit4(" Light Ambient ", &mDirectionalLight.ambient.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Diffuse ", &mDirectionalLight.diffuse.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Light Specular ", &mDirectionalLight.specular.r, ImGuiColorEditFlags_NoAlpha);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Material ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4(" Material Ambient ", &mEarth.material.ambient.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Material Diffuse ", &mEarth.material.diffuse.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Material Specular ", &mEarth.material.specular.r, ImGuiColorEditFlags_NoAlpha);
		ImGui::SliderFloat(" Specular Power ", &mEarth.material.power, 0.0f, 20.0f);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Settings ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(" Use Specular Map ", &mStandardEffect.UseSpecularMap());
		ImGui::Checkbox(" Use Bump Map ", &mStandardEffect.UseDisplacementMap());
		ImGui::Checkbox(" Use Normal Map ", &mStandardEffect.UseNormalMap());
		ImGui::SliderFloat(" Bump Weight ", &mStandardEffect.DisplacementWeight(), 0.0f, 1.0f);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Post Processing ", ImGuiTreeNodeFlags_DefaultOpen))
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

	ImGui::Separator();
	if (ImGui::CollapsingHeader(" Bloom Settings ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt(" Blur Iterations ", &mBlurIteration, 1, 0, 100);
		ImGui::DragFloat(" Blur Saturation ", &mBlurSaturation, 0.001f, 0.0f, 1.0f);
	}
	ImGui::End();

	ImGui::Begin("Render Targets", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Base Render Target");
	ImGui::Image(mBaseRenderTarget.GetRawData(), { 256, 144 });
	ImGui::Text("Bloom Render Target");
	ImGui::Image(mBloomRenderTarget.GetRawData(), { 256, 144 });
	ImGui::Text("Blur Horizontal Render Target");
	ImGui::Image(mBlurHRenderTarget.GetRawData(), { 256, 144 });
	ImGui::Text("Blur Vertical Render Target");
	ImGui::Image(mBlurVRenderTarget.GetRawData(), { 256, 144 });
	ImGui::End();
}

void GameState::RenderScene()
{
	mSkybox.Render(mCamera);

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetProjectionMatrix();

	//Render Earth
	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	mStandardEffect.End();

	//Render Sun
	auto matWorld = Matrix4::Scaling({ 2.0f, 2.0f, 2.0f });
	mTransformPXData.wvp = Transpose(matWorld * matView * matProj);

	mUnlitVertexShader.Bind();
	mUnlitPixelShader.Bind();

	mTransformBufferPX.Update(mTransformPXData);
	mTransformBufferPX.BindVS(0);
	mTransformBufferPX.BindPS(0);
	mSunTexture.BindPS(0);

	mSunMeshBuffer.Render();
}

void GameState::RenderBrightness()
{
	mUnlitVertexShader.Bind();
	mUnlitPixelShader.Bind();

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetProjectionMatrix();

	//Render Earth
	Matrix4 matWorld = mEarth.transform.GetMatrix4();
	mTransformPXData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBufferPX.Update(mTransformPXData);
	mTransformBufferPX.BindVS(0);

	Texture::UnbindPS(0);
	mEarhMeshBuffer.Render();

	//Render Sun
	matWorld = Matrix4::Scaling({ 2.0f, 2.0f, 2.0f });
	mTransformPXData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBufferPX.Update(mTransformPXData);

	mSunTexture.BindPS(0);
	mSunMeshBuffer.Render();
}

void GameState::ApplyBlur()
{
	mGaussianBlurVertexShader.Bind();

	auto graphicsSystem = GraphicsSystem::Get();
	mBlurSettingsData.screenWidth = static_cast<float>(graphicsSystem->GetBackBufferWidth());
	mBlurSettingsData.screenHeight = static_cast<float>(graphicsSystem->GetBackBufferHeight());
	mBlurSettingsData.multiplier = mBlurSaturation;
	mBlurSettingsBuffer.Update(mBlurSettingsData);
	mBlurSettingsBuffer.BindPS(0);

	mSampler.BindPS(0);

	RenderTarget* rt0 = &mBlurHRenderTarget;
	RenderTarget* rt1 = &mBloomRenderTarget;

	for (int i = 0; i < mBlurIteration; ++i)
	{
		mGaussianBlurHPixelShader.Bind();
		rt0->BeginRender();
		rt1->BindPS(0);
		mScreenMeshBuffer.Render();
		Texture::UnbindPS(0);
		rt0->EndRender();

		rt1 = &mBlurVRenderTarget;

		mGaussianBlurVPixelShader.Bind();
		rt1->BeginRender();
		rt0->BindPS(0);
		mScreenMeshBuffer.Render();
		Texture::UnbindPS(0);
		rt1->EndRender();
	}
}

void GameState::PostProcess()
{
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	mPostProcessSettingsBuffer.Update(mPostProcessSettingsData);
	mPostProcessSettingsBuffer.BindPS(0);
	mSampler.BindPS(0);

	mBaseRenderTarget.BindPS(0);
	mBlurVRenderTarget.BindPS(1);
	mScreenMeshBuffer.Render();
	Texture::UnbindPS(0);
}
